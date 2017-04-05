//
// Created by megaxela on 09.12.16.
//

#include <Tools/Logger.h>
#include <QMessageBox>
#include <DriverHolder.h>
#include <TestDriverHolder.h>
#include <Testing/SettingsSystem.h>
#include <Windows/Controllers/ConnectControllers/ConnectTCPTabController.h>
#include <ui_mainwindow.h>
#include <Windows/Controllers/ConnectTabController.h>
#include <Testing/ConnectionsManager/ConnectionsManager.h>

ConnectTCPTabController::ConnectTCPTabController(Ui::MainWindow *ptr, QWidget *parent, QTabWidget *tabWidget)
        : AbstractTabController(ptr, parent, tabWidget),
          m_tcpInterface(ConnectionsManager::instance().getTCPInterface())
{

}

ConnectTCPTabController::~ConnectTCPTabController()
{

}

void ConnectTCPTabController::setupConnections()
{
    connect(ui()->connectionTCPConnectPushButton,
            &QPushButton::clicked,
            this,
            &ConnectTCPTabController::onConnecting);

    connect(ui()->connectionTCPIPLineEdit,
            &QLineEdit::editingFinished,
            this,
            &ConnectTCPTabController::onIPEditingFinished);

    connect(ui()->connectionTCPPortLineEdit,
            &QLineEdit::editingFinished,
            this,
            &ConnectTCPTabController::onPortEditingFinished);
}

void ConnectTCPTabController::configureWidgets()
{
    ui()->connectionTCPIPLineEdit->setValidator(
            new QRegExpValidator(
                    QRegExp("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})"),
                    this
            )
    );

    ui()->connectionTCPPortLineEdit->setValidator(
            new QIntValidator(0, 65535, this)
    );

    ui()->connectionTCPIPLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance().getValue(
                            SettingsSystem::ConnectionIPAddress
                    )
            )
    );

    ui()->connectionTCPPortLineEdit->setText(
            QString::fromStdString(
                    SettingsSystem::instance().getValue(
                            SettingsSystem::ConnectionIPPort
                    )
            )
    );

    if (ui()->connectionTabWidget->currentWidget() == ui()->connectionTCPTab)
    {
        Log("Сейчас открыто TCP подключение.");

        bool ok = false;

        QString ipString = ui()->connectionTCPIPLineEdit->text();
        QString portString = ui()->connectionTCPPortLineEdit->text();

        uint16_t portInt = portString.toUShort(&ok);

        if (!ok)
        {
            Error("Строка " + portString.toStdString() + " числом не является.");
            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    "Строка " + portString + " числом не является."
            );
            return;
        }

        IPv4Address address;

        if (!address.setFromString(ipString.toStdString()))
        {
            Error("Строка " + ipString.toStdString() + " не является валидным ip адресом.");
            QMessageBox::critical(
                    parentWidget(),
                    "Ошибка",
                    "Строка " + ipString + " не является валидным ip адресом."
            );
            return;
        }

        m_tcpInterface->setAddress(address, portInt);

        DriverHolder::driver().setInterface(m_tcpInterface);
        TestDriverHolder::driver().setInterface(m_tcpInterface);
    }
}

void ConnectTCPTabController::onConnecting()
{
    bool ok = false;

    QString ipString = ui()->connectionTCPIPLineEdit->text();
    QString portString = ui()->connectionTCPPortLineEdit->text();

    uint16_t portInt = portString.toUShort(&ok);

    if (!ok)
    {
        Error("Строка " + portString.toStdString() + " числом не является.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Строка " + portString + " числом не является."
        );
        return;
    }

    IPv4Address address;

    if (!address.setFromString(ipString.toStdString()))
    {
        Error("Строка " + ipString.toStdString() + " не является валидным ip адресом.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Строка " + ipString + " не является валидным ip адресом."
        );
        return;
    }

    m_tcpInterface->setAddress(address, portInt);

    DriverHolder::driver().setInterface(m_tcpInterface);
    TestDriverHolder::driver().setInterface(m_tcpInterface);

    if (!m_tcpInterface->openConnection())
    {
        Error("Не удалось подключиться к серверу.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка подключения",
                "Не удалось подключиться к серверу."
        );
        return;
    }

    if (!DriverHolder::driver().protocol()->checkConnection(m_tcpInterface))
    {
        Error("Подключиться к сервеу. Но проверка соединения не удалась.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка подключения",
                "Подключиться к сервеу. Но проверка соединения не удалась."
        );
        return;
    }

    Log("Подключение к IP " + ipString.toStdString() + " на порту " + portString.toStdString() + " успешно.");

    Log("Получение информации по устройству.");
    if (!connectTabController()->receiveDeviceInfo())
    {
        Error("Не удалось получить информацию об устройстве.");
    }
}

void ConnectTCPTabController::onIPEditingFinished()
{
    SettingsSystem::instance().setValue(
            SettingsSystem::ConnectionIPAddress,
            ui()->connectionTCPIPLineEdit->text().toStdString()
    );
}

void ConnectTCPTabController::onPortEditingFinished()
{
    SettingsSystem::instance().setValue(
            SettingsSystem::ConnectionIPPort,
            ui()->connectionTCPPortLineEdit->text().toStdString()
    );
}

ConnectTabController *ConnectTCPTabController::connectTabController()
{
    return (ConnectTabController*) parentController();
}
