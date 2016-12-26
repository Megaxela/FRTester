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
    
    disconnectEditor();

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

    connectEditor();
}



void TablesTabController::onItemEdited(int row, int column)
{
    // Проверка значения
    auto value = ui()->tablesFieldsTableWidget->item(row, column)->text();

    auto fieldStructure = m_currentTableFieldsStructures[row];
    if (fieldStructure.fieldType == FRDriver::FieldStructure::Bin)
    {
        bool ok;
        auto actualValue = value.toULongLong(&ok);

        if (!ok)
        {
            Error("Значение " + value.toStdString() + " не является числом.");
            disconnectEditor();
            ui()->tablesFieldsTableWidget->item(row, column)->setText(m_cellBackup);
            connectEditor();
            return;
        }

        if (fieldStructure.minValue > actualValue || fieldStructure.maxValue < actualValue)
        {
            auto errorString = "Значение " +
                               value.toStdString() +
                               " не попадает в промежуток [" +
                               std::to_string(fieldStructure.minValue) + ", " +
                               std::to_string(fieldStructure.maxValue) + "].";
            Error(errorString);

            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    QString::fromStdString(errorString)
            );
            disconnectEditor();
            ui()->tablesFieldsTableWidget->item(row, column)->setText(m_cellBackup);
            connectEditor();
            return;
        }

        if (!DriverHolder::driver().writeTable(
                getPassword(),
                static_cast<uint8_t>(ui()->tablesFieldsListWidget->currentRow()),
                0,
                static_cast<uint8_t>(row),
                actualValue,
                fieldStructure.numberOfBytes
        ))
        {
            auto errorString = "Запись не удалась. Ошибка №" +
                               std::to_string((int) DriverHolder::driver().getLastError()) +
                               " - " +
                               FRDriver::Converters::errorToString((int) DriverHolder::driver().getLastError());

            Error(errorString);

            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    QString::fromStdString(errorString)
            );
            disconnectEditor();
            ui()->tablesFieldsTableWidget->item(row, column)->setText(m_cellBackup);
            connectEditor();
            return;
        }
    }
    else // String
    {
        if (value.size() > 40)
        {
            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    "Не удалось записать значение. Строка более 40 символов."
            );
            disconnectEditor();
            ui()->tablesFieldsTableWidget->item(row, column)->setText(m_cellBackup);
            connectEditor();
            return;
        }

        QByteArray converted = Codecs::instance().convert("UTF-8", "CP1251", value.toUtf8());

        if (!DriverHolder::driver().writeTable(
                getPassword(),
                static_cast<uint8_t>(ui()->tablesFieldsListWidget->currentRow()),
                0,
                static_cast<uint8_t>(row),
                converted.toStdString()
        ))
        {
            auto errorString = "Запись не удалась. Ошибка №" +
                               std::to_string((int) DriverHolder::driver().getLastError()) +
                               " - " +
                               FRDriver::Converters::errorToString((int) DriverHolder::driver().getLastError());

            Error(errorString);

            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    QString::fromStdString(errorString)
            );
            disconnectEditor();
            ui()->tablesFieldsTableWidget->item(row, column)->setText(m_cellBackup);
            connectEditor();
            return;
        }
    }


}

void TablesTabController::onItemDoubleClicked(QTableWidgetItem *tableWidgetItem)
{
    m_cellBackup = tableWidgetItem->text();
}

void TablesTabController::connectEditor()
{
    // Изменение значения элемента таблицы
    connect(ui()->tablesFieldsTableWidget,
            &QTableWidget::cellChanged,
            this,
            &TablesTabController::onItemEdited);
}

void TablesTabController::disconnectEditor()
{
    // Изменение значения элемента таблицы
    disconnect(ui()->tablesFieldsTableWidget,
               &QTableWidget::cellChanged,
               this,
               &TablesTabController::onItemEdited);
}
