//
// Created by megaxela on 13.12.16.
//

#include <include/DriverHolder.h>
#include <QtWidgets/QMessageBox>
#include <include/Tools/Codecs.h>
#include <include/Tools/Logger.h>
#include "include/Windows/Controllers/TablesTabController.h"
#include "ui_mainwindow.h"

TablesTabController::TablesTabController(Ui::MainWindow *ptr, QWidget *parent)
        : AbstractTabController(ptr, parent, nullptr)
{
    for (uint8_t i = 1; i <= 24; ++i)
    {
        m_tables[i - 1] = i;
    }
}

void TablesTabController::setupConnections()
{
    // Кнопка обновления таблиц
    connect(ui()->tablesUpdateTablesPushButton,
            &QPushButton::clicked,
            this,
            &TablesTabController::onTablesUpdate);

    // Выбор элемента ListView
    connect(ui()->tablesFieldsListWidget,
            &QListWidget::currentRowChanged,
            this,
            &TablesTabController::onCurrentTableChanged);
}

void TablesTabController::configureWidgets()
{
}

void TablesTabController::onTablesUpdate()
{
    Log("Обновление таблиц.");
    if (!checkEverything())
    {
        return;
    }

    auto pwd = getPassword();

    ui()->tablesFieldsListWidget->clear();

    for (int i = 0; i < m_tables.size(); ++i)
    {
        auto structure = DriverHolder::driver().tableStructureRequest(
               pwd,
               m_tables[i]
        );

        m_tableStructures[i] = structure;

        auto qStr = Codecs::instance().convert("CP1251", QByteArray(structure.name.c_str()));

        Log("Найдена таблица #" + std::to_string(m_tables[i]) + " \"" + qStr.toStdString() + "\".");

        ui()->tablesFieldsListWidget->addItem(qStr);
    }
}

bool TablesTabController::checkPassword() const
{
    QString s = ui()->tablesPasswordLineEdit->text();

    bool ok;
    s.toUInt(&ok);

    if (!ok)
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Пароль не является числом."
        );
    }

    return ok;
}

uint32_t TablesTabController::getPassword() const
{
    QString s = ui()->tablesPasswordLineEdit->text();

    return s.toUInt();
}

bool TablesTabController::checkEverything() const
{
    if (!DriverHolder::driver().checkConnection())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Отсутствует соединение с ФР."
        );
        return false;
    }

    return checkPassword();
}

void TablesTabController::onCurrentTableChanged(int index)
{
    if (!checkEverything())
    {
        return;
    }

    auto password = getPassword();

    ui()->tablesFieldsTableWidget->clear();

    ui()->tablesFieldsTableWidget->setColumnCount(2);
    ui()->tablesFieldsTableWidget->setRowCount(m_tableStructures[index].numberOfFields);

    m_currentTableFieldsStructures.clear();

    for (int i = 0; i < m_tableStructures[index].numberOfFields; ++i)
    {
        // Запрос структуры поля
        auto field = DriverHolder::driver().fieldStructureRequest(
                password,
                (uint8_t) m_tables[index],
                (uint8_t) (i + 1)
        );

        if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
        {
            Critical("Ошибка получения структуры поля.");
            break;
        }

        ui()->tablesFieldsTableWidget->setItem(
                i,
                0,
                new QTableWidgetItem(
                        Codecs::instance().convert(
                                "CP1251", QByteArray(field.name.c_str(), 40)
                        )
                )
        );

        m_currentTableFieldsStructures.push_back(field);

        if (field.fieldType == FRDriver::FieldStructure::Bin)
        {
            auto table = DriverHolder::driver().readTableBin(
                    password,
                    (uint8_t) m_tables[index],
                    1,
                    (uint8_t) (i + 1)
            );

            if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
            {
                Critical("Ошибка получения значения поля. Ошибка: " +
                         FRDriver::Converters::errorToString((int) DriverHolder::driver().getLastError()));
                break;
            }

            ui()->tablesFieldsTableWidget->setItem(
                    i,
                    1,
                    new QTableWidgetItem(
                            QString::number(table)
                    )
            );
        }
        else // String
        {
            auto table = DriverHolder::driver().readTableStr(
                    password,
                    (uint8_t) m_tables[index],
                    1,
                    (uint8_t) (i + 1)
            );

            if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
            {
                Critical("Ошибка получения значения поля. Ошибка: " +
                         FRDriver::Converters::errorToString((int) DriverHolder::driver().getLastError()));
                break;
            }

            ui()->tablesFieldsTableWidget->setItem(
                    i,
                    1,
                    new QTableWidgetItem(
                            Codecs::instance().convert(
                                    "CP1251", QByteArray(
                                            table.c_str(),
                                            (int) table.length()
                                    )
                            )
                    )
            );
        }
    }
}
