//
// Created by megaxela on 26.12.16.
//

#include <include/Tools/Logger.h>
#include "include/Windows/Controllers/CommandsControllers/PrintTextPlainTextCommandsTabController.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <include/Tools/Codecs.h>
#include <include/DriverHolder.h>
#include <vector>
#include <QtWidgets/QMessageBox>

PrintTextPlainTextCommandsTabController::PrintTextPlainTextCommandsTabController(Ui::MainWindow *ptr, QWidget *parent)
        : AbstractTabController(ptr, parent, nullptr)
{}

void PrintTextPlainTextCommandsTabController::setupConnections()
{
    // Кнопка печати
    connect(ui()->commandsPrintTextPlainTextPrintPushButton,
            &QPushButton::clicked,
            this,
            &PrintTextPlainTextCommandsTabController::onPrintButtonPressed);

}

void PrintTextPlainTextCommandsTabController::configureWidgets()
{

}

void PrintTextPlainTextCommandsTabController::onPrintButtonPressed()
{
    Log("Начинаем печать текста.");
    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Обрабатываем входные данные для нормальной работы с несимвольными байтами.");

    QString data = ui()->commandsPrintTextPlainTextPlainTextEdit->toPlainText();

    std::vector<std::string> lines;
    parseInput(data, lines);

    for (auto line : lines)
    {
        ByteArray arr((const uint8_t*) line.c_str(), line.size());
        auto converted = Codecs::instance().convert("CP1251", "UTF-8", QByteArray(line.c_str(), line.size()));
        Log("К отправке строка: " + arr.toHex() + " (" + QString(converted).toStdString() + ")");

        if (!DriverHolder::driver().standardStringPrint(
                commandsTabControllerParent()->password(),
                0b111,
                line
        ))
        {
            Error("Не удалось напечатать строку: " +
                  converted.toStdString());

            QMessageBox::critical(parentWidget(), "Ошибка", "Не удалось напечатать строку: " + converted);
            return;
        }

    }

}

void PrintTextPlainTextCommandsTabController::parseInput(const QString &utfString,
                                                         std::vector<std::string> &lines)
{
    QByteArray data = Codecs::instance().convert(
            "UTF-8",
            "CP1251",
            QByteArray(utfString.toStdString().c_str())
    );

    Log(QString(data.toHex()).toStdString());

    std::string result;
    result.reserve(data.length());

    // 0 - символы
    // 1 - найден '\'
    // 2 - найден 'x'
    // 3 - найден первый символ байта

    uint8_t state = 0;
    uint8_t byteValue = 0;
    char remChar = '\x00';
    for (auto ch : data)
    {
        switch (state)
        {
            case 0: // Символы
            {
                if (ch == '\\')
                {
                    state = 1;
                    continue;
                }
                else if (ch == '\n')
                {
                    lines.push_back(result);
                    result.clear();
                    continue;
                }

                result += ch;
                break;
            }
            case 1: // Найден '\'
            {
                if (ch == 'x')
                {
                    state = 2;
                    continue;
                }

                result += '\\';
                result += ch;
                state = 0;

                break;
            }
            case 2: // Найден 'x'
            {
                remChar = ch;
                if (ch >= '0' && ch <= '9')
                {
                    byteValue = (uint8_t) (((uint32_t) remChar - '0') << 4);
                    state = 3;
                }
                else if (ch >= 'a' && ch <= 'f')
                {
                    byteValue = (uint8_t) (((uint32_t) remChar - 'a' + 10) << 4);
                    state = 3;
                }
                else if (ch >= 'A' && ch <= 'F')
                {
                    byteValue = (uint8_t) (((uint32_t) remChar - 'A' + 10) << 4);
                    state = 3;
                }
                else
                {
                    byteValue = 0;
                    result += '\\';
                    result += 'x';
                    result += ch;
                    state = 0;
                }

                break;
            }
            case 3: // Первый байт найден
            {
                state = 0;
                if (ch >= '0' && ch <= '9')
                {
                    byteValue |= (uint8_t) (ch - '0');
                }
                else if (ch >= 'a' && ch <= 'f')
                {
                    byteValue |= (uint8_t) (ch - 'a' + 10);
                }
                else if (ch >= 'A' && ch <= 'F')
                {
                    byteValue |= (uint8_t) (ch - 'A' + 10);
                }
                else
                {
                    byteValue = 0;
                    result += '\\';
                    result += 'x';
                    result += remChar;
                    result += ch;
                    continue;
                }

                result += byteValue;
                byteValue = 0;

                break;
            }
        }

    }

    if (result.size() > 0)
    {
        lines.push_back(result);
    }

}

CommandsTabController *PrintTextPlainTextCommandsTabController::commandsTabControllerParent() const
{
    return (CommandsTabController*) parentController()->parentController()->parentController();
}

