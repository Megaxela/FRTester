//
// Created by megaxela on 13.12.16.
//

#include <QtGui/QFontDatabase>
#include <include/Tools/Logger.h>
#include <include/DriverHolder.h>
#include <include/FRDriver.h>
#include <QPlainTextEdit>
#include "include/Windows/Controllers/CommandsControllers/FNOFDCommandsTabController.h"
#include "include/Windows/Controllers/CommandsTabController.h"
#include "ui_mainwindow.h"

FNOFDCommandsTabController::FNOFDCommandsTabController(Ui::MainWindow *ptr, QWidget *parent)
        : AbstractTabController(ptr, parent, nullptr)
{}

void FNOFDCommandsTabController::setupConnections()
{
    // Кнопка запроса статуса информационного обмена
    connect(ui()->commandsFNOFDGetExchangeStatusPushButton,
            &QPushButton::clicked,
            this,
            &FNOFDCommandsTabController::onGetExchangeStatus);
}

void FNOFDCommandsTabController::configureWidgets()
{
    int id = QFontDatabase::addApplicationFont(":/files/fonts/FiraCode-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont font(family);

    ui()->commandsFNOFDLogPlainTextEdit->setFont(font);
}

void FNOFDCommandsTabController::onGetExchangeStatus()
{
    Log("Выполняем запрос состояния информационного обмена");
    ExcessLog("Проверка наличия соединения");
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    ExcessLog("Запрос на ФР.");
    auto requestResult = DriverHolder::driver().getInformationExchangeStatus(
            commandsTabController()->password()
    );

    ExcessLog("Установка статуса.");
    commandsTabController()->setLastStatus();

    ExcessLog("Проверка на успешность выполнения команды.");
    if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
    {
        return;
    }

    QString stringBuilder;
    QString divider = "==================================================================";

    stringBuilder.append(divider + '\n');
    stringBuilder.append("Статус информационного обмера (флаги): " + QString::number(requestResult.status) + '\n');
    stringBuilder.append("0. Транспортное соединение установлено   : " + yesNo((1 << 0) & requestResult.status) + '\n');
    stringBuilder.append("1. Есть сообщение для передачи на ОФД    : " + yesNo((1 << 1) & requestResult.status) + '\n');
    stringBuilder.append("2. Ожидание ответного сообщения от ОФД   : " + yesNo((1 << 2) & requestResult.status) + '\n');
    stringBuilder.append("3. Есть команда от ОФД                   : " + yesNo((1 << 3) & requestResult.status) + '\n');
    stringBuilder.append("4. Изменились настройки соединения с ОФД : " + yesNo((1 << 4) & requestResult.status) + '\n');
    stringBuilder.append("5. Ожидание ответа на команду от ОФД     : " + yesNo((1 << 5) & requestResult.status) + '\n');
    stringBuilder.append(divider + '\n');
    stringBuilder.append("Состояние чтения сообщения  : " + yesNo(requestResult.readStatus) + "\n");
    stringBuilder.append("Количество сообщений для ОФД: " + QString::number(requestResult.messagesForOfd) + '\n');
    stringBuilder.append("Номер первого документа документа для ОФД: " + QString::number(requestResult.firstDocumentNumber) + '\n');
    stringBuilder.append("Дата и время первого документа для ОФД: " +
                                 dateTime(requestResult.date.day,
                                          requestResult.date.month,
                                          requestResult.date.year,
                                          requestResult.time.hour,
                                          requestResult.time.minute
                                 ) + '\n');

    stringBuilder.append(divider);

    ui()->commandsFNOFDLogPlainTextEdit->setPlainText(stringBuilder);
}

CommandsTabController *FNOFDCommandsTabController::commandsTabController() const
{
    return (CommandsTabController *) parentController()->parentController();
}

QString FNOFDCommandsTabController::yesNo(int v)
{
    return v ? "Да" : "Нет";
}

QString FNOFDCommandsTabController::dateTime(uint8_t d,
                                             uint8_t m,
                                             uint8_t y,
                                             uint8_t hour,
                                             uint8_t min)
{
    return QString::number(d) + '.' + QString::number(m) + ".20" + QString::number(y) + ' ' +
           QString::number(hour) + ':' + QString::number(min);
}


