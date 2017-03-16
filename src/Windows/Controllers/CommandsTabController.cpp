//
// Created by megaxela on 17.11.16.
//

#include <iostream>
#include <QtWidgets/QMessageBox>
#include <Tools/Logger.h>
#include <FRDriver.h>
#include <DriverHolder.h>
#include <Windows/Controllers/CommandsControllers/ReportsCommandsTabController.h>
#include <Windows/Controllers/CommandsControllers/RegisterOperationsV1CommandsTabController.h>
#include <include/Windows/Controllers/CommandsControllers/PrintCommandsTabController.h>
#include <include/Windows/Controllers/CommandsControllers/FNCommandsTabController.h>
#include <include/Windows/Controllers/CommandsControllers/DeviceCommandsTabController.h>
#include <include/Windows/Controllers/CommandsControllers/RegisterCommandsTabController.h>
#include "Windows/Controllers/CommandsControllers/StateCommandsTabController.h"
#include "Windows/Controllers/CommandsTabController.h"
#include "ui_mainwindow.h"

CommandsTabController::CommandsTabController(Ui::MainWindow *ptr,
                                             QWidget* parent,
                                             QTabWidget* tabWidget) :
        AbstractTabController(ptr, parent, tabWidget)
{
    addTabController(
            ui()->commandsStatesTab,
            new StateCommandsTabController(
                    ptr,
                    parent
            )
    );
    addTabController(
            ui()->commandsReportsTab,
            new ReportsCommandsTabController(
                    ptr,
                    parent
            )
    );
    addTabController(
            ui()->commandsRegistrationTab,
            new RegisterCommandsTabController(
                    ptr,
                    parent,
                    ui()->commandsRegistrationTabWidget
            )
    );
    addTabController(
            ui()->commandsPrintTab,
            new PrintCommandsTabController(
                    ptr,
                    parent,
                    ui()->commandsPrintTabWidget
            )
    );
    addTabController(
            ui()->commandsFNTab,
            new FNCommandsTabController(
                    ptr,
                    parent,
                    ui()->commandsFNTabWidget
            )
    );
    addTabController(
            ui()->commandsDeviceTab,
            new DeviceCommandsTabController(
                    ptr,
                    parent,
                    ui()->commandsDeviceTabWidget
            )
    );
}

CommandsTabController::~CommandsTabController()
{

}

void CommandsTabController::setupConnections()
{

}

void CommandsTabController::configureWidgets()
{

}

uint32_t CommandsTabController::password() const
{
    bool ok = false;
    uint32_t result = ui()->commandsPasswordLineEdit->text().toUInt(&ok);

    if (!ok)
    {
        QMessageBox::critical(parentWidget(), "Ошибка", "Неверное значение пароля заданного в поле \"пароль\". Использован пароль 30.");
        Log("Был указан неверный пароль. Используется пароль 30.");
        return 30;
    }

    Log("Используется пароль " + std::to_string(result) + ".");
    return result;
}

void CommandsTabController::setStatus(FRDriver::ErrorCode code, const QString &status)
{
    QString text = "#" + QString::number((int) code) + " " + status;
    ui()->commandsResultLineEdit->setText(text);
    Log("Результат: " + text.toStdString());
}

void CommandsTabController::setLastStatus()
{
    setStatus(
            DriverHolder::driver().getLastError(),
            QString::fromStdString(
                    FRDriver::Converters::errorToString(
                            (int) DriverHolder::driver().getLastError()
                    )
            )
    );
}

bool CommandsTabController::checkConnectionWithDevice()
{
    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Log("В данный момент времени соединение отсутствует. Пробуем подключиться.");

        if (DriverHolder::driver().physicalInterface() == nullptr ||
            !DriverHolder::driver().physicalInterface()->openConnection())
        {
            Error("Соединение с ФР отсутствует.");
            QMessageBox::critical(parentWidget(), "Ошибка", "Соединение с ФР отсутствует.");
            return false;
        }
        else
        {
            Log("Соединение установлено.");
            return true;
        }
    }
    ExcessLog("Соединение просутствует.");

    return true;
}
