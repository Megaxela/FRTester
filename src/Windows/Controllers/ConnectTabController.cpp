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
#include <Tools/Codecs.h>
#include <Windows/Widgets/QConnectionsListWidgetItem.h>
#include <QMessageBox>
#include <QTime>

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
    connect(findControllers<ConnectCOMTabController>()[0],
            &ConnectCOMTabController::connectionAdded,
            this,
            &ConnectTabController::onConnectionAdded);

    connect(findControllers<ConnectTCPTabController>()[0],
            &ConnectTCPTabController::connectionAdded,
            this,
            &ConnectTabController::onConnectionAdded);

    connect(ui()->connectionFlashingCompletePushButton,
            &QPushButton::clicked,
            this,
            &ConnectTabController::onFlashCompletePushButtonPressed);

    connect(ui()->connectionFlashingZeroingPushButton,
            &QPushButton::clicked,
            this,
            &ConnectTabController::onZeroingPushButtonPressed);

    connect(ui()->connectionFlashingSetTimePushButton,
            &QPushButton::clicked,
            this,
            &ConnectTabController::onSetTimePushButtonPressed);
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

void ConnectTabController::onConnectionAdded(std::shared_ptr<Connection> connection)
{
    // Добавляем соединение в ListWidget
    ui()->connectionConnectionsListWidget->addItem(
            new QConnectionsListWidgetItem(
                    ui()->connectionConnectionsListWidget,
                    connection
            )
    );
}

void ConnectTabController::onFlashCompletePushButtonPressed()
{
    if (!DriverHolder::driver().checkConnection())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Отсутствует соединение с ККТ"
        );
        return;
    }

    onZeroingPushButtonPressed();
    onSetTimePushButtonPressed();
}

void ConnectTabController::onZeroingPushButtonPressed()
{
    if (!DriverHolder::driver().checkConnection())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Отсутствует соединение с ККТ"
        );
        return;
    }

    if (!DriverHolder::driver().technologicalReset())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Не удалось выполнить тех. обнуление: " +
                QString::fromStdString(
                        FRDriver::Converters::errorToString(
                                (int) DriverHolder::driver().getLastError()
                        )
                )
        );
    }
}

void ConnectTabController::onSetTimePushButtonPressed()
{
    if (!DriverHolder::driver().checkConnection())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Отсутствует соединение с ККТ"
        );
        return;
    }

    auto pwdString = ui()->connectionFlashingPasswordLineEdit->text();

    bool result = false;
    auto pwd = pwdString.toUInt(&result);

    if (!result)
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Для установки даты и времени требуется пароль администратора."
        );

        return;
    }

    QDateTime dateTime = QDateTime::currentDateTime();

    if (!DriverHolder::driver().dateProgramming(
            pwd,
            static_cast<uint8_t>(dateTime.date().day()),
            static_cast<uint8_t>(dateTime.date().month()),
            static_cast<uint8_t>(dateTime.date().year())
    ))
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Не удалось установить дату: " +
                QString::fromStdString(
                        FRDriver::Converters::errorToString(
                                (int) DriverHolder::driver().getLastError()
                        )
                )
        );

        return;
    }

    if (!DriverHolder::driver().dateConfirm(
            pwd,
            static_cast<uint8_t>(dateTime.date().day()),
            static_cast<uint8_t>(dateTime.date().month()),
            static_cast<uint8_t>(dateTime.date().year())
    ))
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Не удалось подтвердить дату: " +
                QString::fromStdString(
                        FRDriver::Converters::errorToString(
                                (int) DriverHolder::driver().getLastError()
                        )
                )
        );

        return;
    }

    if (!DriverHolder::driver().timeProgramming(
            pwd,
            static_cast<uint8_t>(dateTime.time().hour()),
            static_cast<uint8_t>(dateTime.time().minute()),
            static_cast<uint8_t>(dateTime.time().second())
    ))
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Не удалось подтвердить дату: " +
                QString::fromStdString(
                        FRDriver::Converters::errorToString(
                                (int) DriverHolder::driver().getLastError()
                        )
                )
        );

        return;
    }
}
