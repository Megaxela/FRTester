//
// Created by megaxela on 17.11.16.
//

#include "Windows/Controllers/ConnectControllers/ConnectCOMTabController.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
#include <Tools/Logger.h>
#include <DriverHolder.h>
#include <QMessageBox>
#include <Implementation/DefaultProtocol.h>
#include <TestDriverHolder.h>
#include <Testing/SettingsSystem.h>
#include <Windows/Controllers/ConnectTabController.h>
#include <Testing/ConnectionsManager/ConnectionsManager.h>
#include <Testing/ConnectionsManager/COMConnection.h>

ConnectCOMTabController::ConnectCOMTabController(Ui::MainWindow *ptr, QWidget* parent) :
    AbstractTabController(ptr, parent, nullptr),
    m_comInterface(ConnectionsManager::instance().getCOMInterface())
{
    m_comInterface->setByteSendTime(0);
}

ConnectCOMTabController::~ConnectCOMTabController()
{

}

void ConnectCOMTabController::setupConnections()
{
    // Кнопка "Соединиться"
    connect(ui()->connectionCOMConnectPushButton,
            &QPushButton::clicked,
            this,
            &ConnectCOMTabController::onConnecting);

    // Завершение редактирования COM порта
    connect(ui()->connectionCOMDeviceLineEdit,
            &QLineEdit::editingFinished,
            this,
            &ConnectCOMTabController::onDeviceEditingFinished);

    // Завершение выбора скорости обмена
    connect(ui()->connectionCOMBaudRateComboBox,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &ConnectCOMTabController::onBaudRateEditingFinished);
}

void ConnectCOMTabController::configureWidgets()
{
    // Добавление элементов в скорости
    {
        ui()->connectionCOMBaudRateComboBox->addItem("4000000");
        ui()->connectionCOMBaudRateComboBox->addItem("3500000");
        ui()->connectionCOMBaudRateComboBox->addItem("3000000");
        ui()->connectionCOMBaudRateComboBox->addItem("2500000");
        ui()->connectionCOMBaudRateComboBox->addItem("2000000");
        ui()->connectionCOMBaudRateComboBox->addItem("1500000");
        ui()->connectionCOMBaudRateComboBox->addItem("1152000");
        ui()->connectionCOMBaudRateComboBox->addItem("1000000");
        ui()->connectionCOMBaudRateComboBox->addItem("921600");
        ui()->connectionCOMBaudRateComboBox->addItem("576000");
        ui()->connectionCOMBaudRateComboBox->addItem("500000");
        ui()->connectionCOMBaudRateComboBox->addItem("460800");
        ui()->connectionCOMBaudRateComboBox->addItem("230400");
        ui()->connectionCOMBaudRateComboBox->addItem("115200");
        ui()->connectionCOMBaudRateComboBox->addItem("57600");
        ui()->connectionCOMBaudRateComboBox->addItem("38400");
        ui()->connectionCOMBaudRateComboBox->addItem("19200");
        ui()->connectionCOMBaudRateComboBox->addItem("9600");
        ui()->connectionCOMBaudRateComboBox->addItem("4800");
        ui()->connectionCOMBaudRateComboBox->addItem("2400");
        ui()->connectionCOMBaudRateComboBox->addItem("1800");
        ui()->connectionCOMBaudRateComboBox->addItem("1200");
        ui()->connectionCOMBaudRateComboBox->addItem("600");
        ui()->connectionCOMBaudRateComboBox->addItem("300");
        ui()->connectionCOMBaudRateComboBox->addItem("200");
        ui()->connectionCOMBaudRateComboBox->addItem("150");
        ui()->connectionCOMBaudRateComboBox->addItem("134");
        ui()->connectionCOMBaudRateComboBox->addItem("110");
        ui()->connectionCOMBaudRateComboBox->addItem("75");
        ui()->connectionCOMBaudRateComboBox->addItem("50");
        ui()->connectionCOMBaudRateComboBox->addItem("0");
    }

    ui()->connectionCOMDeviceLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance()
                            .getValue(
                                    SettingsSystem::ConnectionCOMPort
                            )
            )
    );

    try
    {
        ui()->connectionCOMBaudRateComboBox->setCurrentIndex(
                std::stoi(
                        SettingsSystem::instance()
                                .getValue(
                                        SettingsSystem::ConnectionCOMSpeed
                                )
                )
        );
    }
    catch (std::invalid_argument e)
    {
        Warning("Не удалось получить скорость обмена с COM портом.");
    }

    // todo: Перевести на диалект контроллера
    if (ui()->connectionTabWidget->currentWidget() == ui()->connectionCOMTab)
    {
        Log("Сейчас открыто COM подключение.");
        QString deviceName = ui()->connectionCOMDeviceLineEdit->text();
        QString baudRate   = ui()->connectionCOMBaudRateComboBox->currentText();

        if (deviceName.isEmpty() || baudRate.isEmpty())
        {
            return;
        }

        m_comInterface->setDevice(deviceName.toStdString());
        m_comInterface->setBaudRate(baudRate.toInt());

        DriverHolder::driver().setInterface(m_comInterface);
        TestDriverHolder::driver().setInterface(m_comInterface);
    }
}

void ConnectCOMTabController::onConnecting()
{
    QString deviceName = ui()->connectionCOMDeviceLineEdit->text();
    QString baudRate   = ui()->connectionCOMBaudRateComboBox->currentText();

    if (deviceName.isEmpty() || baudRate.isEmpty())
    {
        Error("Попытка подключиться по COM порту с пустым названием или пустой скоростью.");
        return;
    }

    Log("Подключение по COM порту.");

    m_comInterface->setDevice(deviceName.toStdString());
    m_comInterface->setBaudRate(baudRate.toInt());

    DriverHolder::driver().setInterface(m_comInterface);
    TestDriverHolder::driver().setInterface(m_comInterface);

    if (!m_comInterface->openConnection())
    {
        Error("Не удалось подключиться к порту.");
        QMessageBox::critical(parentWidget(), "Ошибка подключения", "Не удалось подключиться к порту.");
        return;
    }

    if (!DriverHolder::driver().protocol()->checkConnection(m_comInterface))
    {
        Error("К порту подключиться удалось, но соединение с ФР установить не удалось.");
        QMessageBox::critical(parentWidget(), "Ошибка подключения", "К порту подключиться удалось, но соединение с ФР установить не удалось.");
        return;
    }

    Log("Успешное подключение к COM порту №" + deviceName.toStdString());

    Log("Получение информации по устройству.");
    if (!connectTabController()->receiveDeviceInfo())
    {
        Error("Не удалось получить информацию об устройстве.");
    }
}


void ConnectCOMTabController::onDeviceEditingFinished()
{
    SettingsSystem::instance().setValue(
            SettingsSystem::ConnectionCOMPort,
            ui()->connectionCOMDeviceLineEdit->text().toStdString()
    );
}

void ConnectCOMTabController::onBaudRateEditingFinished()
{
    SettingsSystem::instance().setValue(
            SettingsSystem::ConnectionCOMSpeed,
            std::to_string(
                    ui()->connectionCOMBaudRateComboBox->currentIndex()
            )
    );
}

void ConnectCOMTabController::onConnectionAdd()
{
    QString device = ui()->connectionCOMDeviceLineEdit->text();

    if (device.isEmpty())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Невозможно добавить соединение с пустым устройством."
        );

        return;
    }

    QString baudRate = ui()->connectionCOMBaudRateComboBox->currentText();

    if (baudRate.isEmpty())
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Невозможно добавить соединение с пустой скоростью."
        );

        return;
    }

    auto connection = std::shared_ptr<COMConnection>();

    connection->setDevice(device.toStdString());
    connection->setSpeed(baudRate.toUInt());

    ConnectionsManager::instance().addConnection(connection);

    emit connectionAdded(connection);
}

ConnectTabController *ConnectCOMTabController::connectTabController()
{
    return (ConnectTabController*) parentController();
}
