//
// Created by megaxela on 21.11.16.
//

#include <Tools/Logger.h>
#include <include/DriverHolder.h>
#include <QtWidgets/QMessageBox>
#include <include/Tools/TypeConverters.h>
#include "include/Windows/Controllers/CommandsControllers/RegisterCommandsTabController.h"

RegisterCommandsTabController::RegisterCommandsTabController(Ui::MainWindow *ptr,
                                                             QWidget *parent,
                                                             AbstractTabController *parentController) :
    AbstractTabController(),
    m_ui(ptr),
    m_parent(parent),
    m_parentController(dynamic_cast<CommandsTabController*>(parentController))
{

}

void RegisterCommandsTabController::setupConnections()
{
    // Кнопка покупки
    connect(m_ui->commandsRegistrationSellPushButton,
            &QPushButton::clicked,
            this,
            &RegisterCommandsTabController::onSellButtonPressed);

    // Кнопка продажи
    connect(m_ui->commandsRegistrationBuyPushButton,
            &QPushButton::clicked,
            this,
            &RegisterCommandsTabController::onBuyButtonPressed);

    // Кнопка аннулирования чека
    connect(m_ui->commandsRegisterCancelPushButton,
            &QPushButton::clicked,
            this,
            &RegisterCommandsTabController::onCheckCancel);

    // Кнопка закрытия чека
    connect(m_ui->commandsRegisterClosePushButton,
            &QPushButton::clicked,
            this,
            &RegisterCommandsTabController::onCheckClose);
}

void RegisterCommandsTabController::configureWidgets()
{

}

void RegisterCommandsTabController::onSellButtonPressed()
{
    Log("Выполняем продажу");

    ExcessLog("Проверка входных данных");

    bool ok = false;

    short changeShort = m_ui->commandsRegistrationChangeLineEdit->text().toUShort(&ok);

    if (changeShort > 255)
    {
        Error("Неверное значение смены.");
        QMessageBox::critical(m_parent, "Ошибка", "Неверное значение смены.");
        return;
    }

    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(m_parent, "Ошибка", "Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().sell(
            m_parentController->password(),
            static_cast<uint32_t>(m_ui->commandsRegistrationCountDoubleSpinBox->value() * 100),
            static_cast<uint32_t>(m_ui->commandsRegistrationPriceDoubleSpinBox->value() * 100),
            static_cast<uint8_t>(changeShort),
            static_cast<uint8_t>(m_ui->commandsRegistration1TaxSpinBox->value()),
            static_cast<uint8_t>(m_ui->commandsRegistration2TaxSpinBox->value()),
            static_cast<uint8_t>(m_ui->commandsRegistration3TaxSpinBox->value()),
            static_cast<uint8_t>(m_ui->commandsRegistration4TaxSpinBox->value()),
            "TEST"
    ))
    {
        Error("Не удалось совершить продажу.");
    }

    m_parentController->setLastStatus();
}

void RegisterCommandsTabController::onBuyButtonPressed()
{
    Log("Выполняем покупку");

    QMessageBox::critical(m_parent, "Ошибка", "Еще не реализовано");
    return;

    ExcessLog("Проверка входных данных");

    bool ok = false;

    uint64_t count = static_cast<uint32_t>(m_ui->commandsRegistrationCountDoubleSpinBox->value() * 100);
    uint64_t price = static_cast<uint32_t>(m_ui->commandsRegistrationPriceDoubleSpinBox->value() * 100);

    short changeShort = m_ui->commandsRegistrationChangeLineEdit->text().toUShort();

    if (changeShort > 255)
    {
        Error("Неверное значение смены.");
        QMessageBox::critical(m_parent, "Ошибка", "Неверное значение смены.");
        return;
    }

    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(m_parent, "Ошибка", "Соединение с ФР отсутствует.");
        return;
    }



    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().sell(
            m_parentController->password(),
            count,
            price,
            static_cast<uint8_t>(changeShort),
            m_ui->commandsRegistration1TaxSpinBox->value(),
            m_ui->commandsRegistration2TaxSpinBox->value(),
            m_ui->commandsRegistration3TaxSpinBox->value(),
            m_ui->commandsRegistration4TaxSpinBox->value(),
            "TEST"
    ))
    {
        Error("Не удалось совершить продажу.");
    }

    m_parentController->setLastStatus();
}

void RegisterCommandsTabController::onSellReturnButtonPressed()
{
    QMessageBox::critical(m_parent, "Ошибка", "Еще не реализовано");
}

void RegisterCommandsTabController::onBuyReturnButtonPressed()
{
    QMessageBox::critical(m_parent, "Ошибка", "Еще не реализовано");
}

void RegisterCommandsTabController::onCheckClose()
{
    Log("Выполняем закрытие чека.");
    ExcessLog("Проверка налиия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(m_parent, "Ошибка", "Соединение с ФР отсутствует.");
        return;
    }

    try
    {
        ExcessLog("Запрос на ФР.");
        DriverHolder::driver().closeCheck(
                m_parentController->password(),
                TypeConverters::toUint64(m_ui->commandsRegistration1MoneyLineEdit->text()),
                TypeConverters::toUint64(m_ui->commandsRegistration2MoneyLineEdit->text()),
                TypeConverters::toUint64(m_ui->commandsRegistration3MoneyLineEdit->text()),
                TypeConverters::toUint64(m_ui->commandsRegistration4MoneyLineEdit->text()),
                0,
                m_ui->commandsRegistration1TaxSpinBox->value(),
                m_ui->commandsRegistration2TaxSpinBox->value(),
                m_ui->commandsRegistration3TaxSpinBox->value(),
                m_ui->commandsRegistration4TaxSpinBox->value(),
                "TEST"
        );

        ExcessLog("Установка статуса.");
        m_parentController->setLastStatus();
    }
    catch (ConvertException& e)
    {
        QMessageBox::critical(
                m_parent,
                "Ошибка",
                QString::fromStdString(
                        "Неверное значение одного из полей. \n" +
                        std::string(e.what())
                )
        );
        return;
    }
}

void RegisterCommandsTabController::onCheckCancel()
{
    Log("Выполняем закрытие чека.");
    ExcessLog("Проверка налиия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(m_parent, "Ошибка", "Соединение с ФР отсутствует.");
        return;
    }

    try
    {
        ExcessLog("Запрос на ФР.");
        DriverHolder::driver().cancelCheck(
                m_parentController->password()
        );

        ExcessLog("Установка статуса.");
        m_parentController->setLastStatus();
    }
    catch (ConvertException& e)
    {
        QMessageBox::critical(
                m_parent,
                "Ошибка",
                QString::fromStdString(
                        "Неверное значение одного из полей. \n" +
                        std::string(e.what())
                )
        );
        return;
    }
}

