//
// Created by megaxela on 11.01.17.
//

#include <Windows/Controllers/CommandsControllers/DeviceActionsCommandsTabController.h>
#include <ui_mainwindow.h>
#include <DriverHolder.h>
#include <QMessageBox>
#include <Tools/Logger.h>
#include <Windows/Controllers/CommandsTabController.h>

DeviceActionsCommandsTabController::DeviceActionsCommandsTabController(Ui::MainWindow *ptr, QWidget *parent) :
    AbstractTabController(ptr, parent, nullptr)
{

}

void DeviceActionsCommandsTabController::setupConnections()
{
    connect(ui()->commandsDeviceActionPingPushButton,
            &QPushButton::clicked,
            this,
            &DeviceActionsCommandsTabController::onPingButtonPressed);

    connect(ui()->commandsDeviceActionRequestPushButton,
            &QPushButton::clicked,
            this,
            &DeviceActionsCommandsTabController::onCommandRequestButtonPressed);

    connect(ui()->commandsDeviceActionRebootPushButton,
            &QPushButton::clicked,
            this,
            &DeviceActionsCommandsTabController::onRebootButtonPressed);
}

void DeviceActionsCommandsTabController::configureWidgets()
{

}

void DeviceActionsCommandsTabController::onPingButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    QString uri = ui()->commandsDeviceActionPingLineEdit->text();

    if (uri.isEmpty())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "IP адрес/Домен пуст."
        );

        return;
    }

    auto pingResult = DriverHolder::driver().ping(uri.toStdString());

    commandsTabController()->setLastStatus();

    if (DriverHolder::driver().getLastError() == FRDriver::ErrorCode::NoError)
    {
        if (!pingResult.success)
        {
            ui()->commandsDeviceActionPingResutLabel->setText("Нет соединения.");
        }
        else
        {
            ui()->commandsDeviceActionPingResutLabel->setText(
                    QString::number(pingResult.time) + "ms"
            );
        }
    }
}

void DeviceActionsCommandsTabController::onCommandRequestButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    QString s = ui()->commandsDeviceActionRequestLineEdit->text();

    Log("Попытка преобразовать строку " + s.toStdString() + " в байты.");

    s.remove(' ');

    ByteArray byteArray = ByteArray::fromHex(s.toStdString());

    if (byteArray.size() != static_cast<uint32_t>(s.size() / 2))
    {
        Error("Не удалось преобразовать строку.");
        ui()->commandsDeviceActionResponseLineEdit->setText(
                "Неверный запрос. Не удалось преобразовать запрос в байты."
        );
        return;
    }

    auto response = DriverHolder::driver().sendRaw(byteArray);

    if (response.empty())
    {
        Error("Не удалось получить ответ от устройства.");
        ui()->commandsDeviceActionResponseLineEdit->setText(
                "Не удалось получить ответ от устройства."
        );
        return;
    }

    auto hexRepresentation = response.toHex();

    for (uint32_t i = (uint32_t) (hexRepresentation.size() - 1);
         i > 0;
         --i)
    {
        if (i % 2 == 0)
        {
            hexRepresentation.insert(i, " ");
        }
    }

    ui()->commandsDeviceActionResponseLineEdit->setText(
            QString::fromStdString(hexRepresentation)
    );
}

CommandsTabController *DeviceActionsCommandsTabController::commandsTabController()
{
    return (CommandsTabController*) parentController()->parentController()->parentController();
}

void DeviceActionsCommandsTabController::onRebootButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    Log("Перезапускаем устройство.");

    DriverHolder::driver().reboot();

    commandsTabController()->setLastStatus();
}

void DeviceActionsCommandsTabController::onFirmwareNumberWriteButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    Log("Проверяем входные данные.");

    auto newKey = ui()->commandsDeviceActionNewKeyLineEdit->text();
    auto oldLicense = ui()->commandsDeviceActionOldLicenseLineEdit->text();

    if (newKey.size() < 16)
    {
        Error("Ключ слишком мал.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Новый ключ должен иметь 16 символов. (" + newKey + ")."
        );
        return;
    }

    if (oldLicense.size() < 8)
    {
        Error("Лицензия слишком мала.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Старая лицензия должна иметь 8 символов. (" + oldLicense + ")."
        );
        return;
    }

    DriverHolder::driver().changeFirmware(
            newKey.toStdString(),
            oldLicense.toStdString()
    );

    commandsTabController()->setLastStatus();
}
