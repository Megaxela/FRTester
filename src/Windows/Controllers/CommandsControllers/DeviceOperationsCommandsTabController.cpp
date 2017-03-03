//
// Created by megaxela on 12.01.17.
//

#include "include/Windows/Controllers/CommandsControllers/DeviceOperationsCommandsTabController.h"
#include <Windows/Controllers/CommandsTabController.h>
#include <include/DriverHolder.h>
#include <ui_mainwindow.h>

DeviceOperationsCommandsTabController::DeviceOperationsCommandsTabController(Ui::MainWindow *ptr, QWidget *parent) :
    AbstractTabController(ptr, parent, nullptr),
    m_registerNames(

    )
{

}

void DeviceOperationsCommandsTabController::setupConnections()
{
    connect(ui()->commandsDeviceOperationsReadPushButton,
            &QPushButton::clicked,
            this,
            &DeviceOperationsCommandsTabController::onReadButtonPressed);

    connect(ui()->commandsDeviceOperationsReadOnePushButton,
            &QPushButton::clicked,
            this,
            &DeviceOperationsCommandsTabController::onCurrentReadButtonPressed);
}

void DeviceOperationsCommandsTabController::configureWidgets()
{

}

CommandsTabController *DeviceOperationsCommandsTabController::commandsTabController()
{
    return (CommandsTabController*) parentController()->parentController()->parentController();
}

void DeviceOperationsCommandsTabController::onReadButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    ui()->commandsDeviceOperationsPlainText->clear();
    for (uint32_t index = 0; index < 256; ++index)
    {
        // Считывание значение регистра
        auto result = DriverHolder::driver().operatingRegisterRequest(
                commandsTabController()->password(),
                (uint8_t) index
        );

        if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
        {
            commandsTabController()->setLastStatus();
            return;
        }

        auto line = QString::number(index);
        for (int i = 0; i < 3 - line.size(); ++i)
        {
            line = ' ' + line;
        }

        line += ". ";
        line += QString::fromStdString(m_registerNames[index]);
        line += ": ";
        line += QString::number(result);

        ui()->commandsDeviceOperationsPlainText->appendPlainText(
                line
        );
    }

    commandsTabController()->setLastStatus();
}

void DeviceOperationsCommandsTabController::onCurrentReadButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    auto result = DriverHolder::driver().operatingRegisterRequest(
            commandsTabController()->password(),
            static_cast<uint8_t>(ui()->commandsDeviceOperationsCurrentNumberSpinBox->value())
    );

    if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
    {
        commandsTabController()->setLastStatus();
        return;
    }

    ui()->commandsDeviceOperationsCurrentResultLineEdit->setText(
            QString::number(result)
    );
}
