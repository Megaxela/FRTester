//
// Created by megaxela on 08.12.16.
//

#include <Windows/Controllers/ConnectTabController.h>
#include <Windows/Controllers/ConnectControllers/ConnectCOMTabController.h>
#include <Windows/Controllers/ConnectControllers/ConnectTCPTabController.h>
#include <TestDriverHolder.h>
#include <Implementation/DefaultProtocol.h>
#include <DriverHolder.h>
#include <Tools/Logger.h>
#include <ui_mainwindow.h>
#include <include/Tools/Codecs.h>

ConnectTabController::ConnectTabController(Ui::MainWindow *ptr, QWidget *parent, QTabWidget *tabWidget)
        : AbstractTabController(ptr, parent, tabWidget)
{
    auto proto = std::make_shared<DefaultProtocol>();

    TestDriverHolder::driver().setProtocol(proto);
    DriverHolder::driver().setProtocol(proto);

    addTabController(
            ui()->connectionCOMTab,
            new ConnectCOMTabController(
                    ui(),
                    parentWidget()
            )
    );

    addTabController(
            ui()->connectionTCPTab,
            new ConnectTCPTabController(
                    ui(),
                    parentWidget()
            )
    );
}

ConnectTabController::~ConnectTabController()
{

}

void ConnectTabController::setupConnections()
{

}

void ConnectTabController::configureWidgets()
{

}

void ConnectTabController::tabSelected()
{

}

bool ConnectTabController::receiveDeviceInfo()
{
    auto deviceType = DriverHolder::driver().getDeviceType();

    if (DriverHolder::driver().getLastError() != FRDriver::ErrorCode::NoError)
    {
        Log("Не удалось получить информацию об устройстве.");

        ui()->connectionInfoDeviceNameLabel->setText("Ошибка");
        ui()->connectionInfoProtocolVersionLabel->setText("Ошибка");
        ui()->connectionInfoDeviceTypeLabel->setText("Ошибка");
        ui()->connectionInfoDeviceModelLabel->setText("Ошибка");
        ui()->connectionInfoLanguageLabel->setText("Ошибка");

        return false;
    }

    ui()->connectionInfoDeviceNameLabel->setText(
            Codecs::instance().convert(
                    "cp1251",
                    "utf-8",
                    QByteArray::fromStdString(deviceType.deviceName)
            )
    );
    ui()->connectionInfoProtocolVersionLabel->setText(
            QString::number(deviceType.protocolVersion) +
            '.' +
            QString::number(deviceType.protocolSubVersion)
    );
    ui()->connectionInfoDeviceTypeLabel->setText(
            QString::number(deviceType.deviceType) +
            '.' +
            QString::number(deviceType.deviceSubType)
    );
    ui()->connectionInfoDeviceModelLabel->setText(
            QString::number(deviceType.deviceModel)
    );
    ui()->connectionInfoLanguageLabel->setText(
            QString::fromStdString(
                    FRDriver::Converters::deviceLanguageToString(
                            deviceType.language
                    )
            )
    );

    return true;
}
