//
// Created by megaxela on 17.11.16.
//

#include "Windows/Controllers/ConnectTabController.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
#include <Tools/Logger.h>
#include <include/DriverHolder.h>
#include <QtWidgets/QMessageBox>
#include <Implementation/DefaultProtocol.h>
#include <include/TestDriverHolder.h>

ConnectTabController::ConnectTabController(Ui::MainWindow *ptr, QWidget* parent) :
    AbstractTabController(),
    m_comInterface(std::make_shared<COMInterface>()),
    m_parent(parent)
{
    m_ui = ptr;

    Log("Установлен стандартный протокол.");

    auto proto = std::make_shared<DefaultProtocol>();

    DriverHolder::driver().setProtocol(proto);
    DriverHolder::driver().setInterface(m_comInterface);
    TestDriverHolder::driver().setProtocol(proto);
    TestDriverHolder::driver().setInterface(m_comInterface);

    m_comInterface->setByteSendTime(0);
}

ConnectTabController::~ConnectTabController()
{

}

void ConnectTabController::setupConnections()
{
    // Кнопка "Соединиться"
    connect(m_ui->connectionConnectPushButton, &QPushButton::clicked, this, &ConnectTabController::onConnecting);
}

void ConnectTabController::configureWidgets()
{
    // Добавление элементов в скорости
    {
        m_ui->connectionCOMBaudRateComboBox->addItem("0");
        m_ui->connectionCOMBaudRateComboBox->addItem("50");
        m_ui->connectionCOMBaudRateComboBox->addItem("75");
        m_ui->connectionCOMBaudRateComboBox->addItem("110");
        m_ui->connectionCOMBaudRateComboBox->addItem("134");
        m_ui->connectionCOMBaudRateComboBox->addItem("150");
        m_ui->connectionCOMBaudRateComboBox->addItem("200");
        m_ui->connectionCOMBaudRateComboBox->addItem("300");
        m_ui->connectionCOMBaudRateComboBox->addItem("600");
        m_ui->connectionCOMBaudRateComboBox->addItem("1200");
        m_ui->connectionCOMBaudRateComboBox->addItem("1800");
        m_ui->connectionCOMBaudRateComboBox->addItem("2400");
        m_ui->connectionCOMBaudRateComboBox->addItem("4800");
        m_ui->connectionCOMBaudRateComboBox->addItem("9600");
        m_ui->connectionCOMBaudRateComboBox->addItem("19200");
        m_ui->connectionCOMBaudRateComboBox->addItem("38400");
        m_ui->connectionCOMBaudRateComboBox->addItem("57600");
        m_ui->connectionCOMBaudRateComboBox->addItem("115200");
        m_ui->connectionCOMBaudRateComboBox->addItem("230400");
        m_ui->connectionCOMBaudRateComboBox->addItem("460800");
        m_ui->connectionCOMBaudRateComboBox->addItem("500000");
        m_ui->connectionCOMBaudRateComboBox->addItem("576000");
        m_ui->connectionCOMBaudRateComboBox->addItem("921600");
        m_ui->connectionCOMBaudRateComboBox->addItem("1000000");
        m_ui->connectionCOMBaudRateComboBox->addItem("1152000");
        m_ui->connectionCOMBaudRateComboBox->addItem("1500000");
        m_ui->connectionCOMBaudRateComboBox->addItem("2000000");
        m_ui->connectionCOMBaudRateComboBox->addItem("2500000");
        m_ui->connectionCOMBaudRateComboBox->addItem("3000000");
        m_ui->connectionCOMBaudRateComboBox->addItem("3500000");
        m_ui->connectionCOMBaudRateComboBox->addItem("4000000");
    }
}

void ConnectTabController::onConnecting()
{
    QString name = m_ui->connectionTabWidget->currentWidget()->objectName();

    if (name == "connectionCOMTab")
    {
        connectCOM();
    }
}

void ConnectTabController::connectCOM()
{
    QString deviceName = m_ui->connectionCOMDeviceLineEdit->text();
    QString baudRate = m_ui->connectionCOMBaudRateComboBox->currentText();

    if (deviceName.isEmpty() || baudRate.isEmpty())
    {
        Error("Попытка подключиться по COM порту с пустым названием или пустой скоростью.");
        return;
    }

    Log("Подключение по COM порту.");

    m_comInterface->setDevice(deviceName.toStdString());
    m_comInterface->setBaudRate(baudRate.toInt());

    DriverHolder::driver().setInterface(m_comInterface);

    if (!m_comInterface->openConnection())
    {
        Error("Не удалось подключиться к порту.");
        QMessageBox::critical(m_parent, "Ошибка подключения", "Не удалось подключиться к порту.");
        return;
    }

    if (!DriverHolder::driver().protocol()->checkConnection(m_comInterface))
    {
        Error("К порту подключиться удалось, но соединение с ФР установить не удалось.");
        QMessageBox::critical(m_parent, "Ошибка подключения", "К порту подключиться удалось, но соединение с ФР установить не удалось.");
        return;
    }

    Log("Успешное подключение к COM порту №" + deviceName.toStdString());
}

