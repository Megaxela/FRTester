//
// Created by megaxela on 21.11.16.
//

#include <Tools/Logger.h>
#include <include/DriverHolder.h>
#include <QtWidgets/QMessageBox>
#include <include/Tools/TypeConverters.h>
#include "include/Windows/Controllers/CommandsControllers/RegisterCommandsTabController.h"

RegisterCommandsTabController::RegisterCommandsTabController(Ui::MainWindow *ptr,
                                                             QWidget *parent) :
    AbstractTabController(ptr, parent, nullptr)
{

}

void RegisterCommandsTabController::setupConnections()
{
    // Кнопка покупки
    connect(ui()->commandsRegistrationSellPushButton,
            &QPushButton::clicked,
            this,
            &RegisterCommandsTabController::onSellButtonPressed);

    // Кнопка продажи
    connect(ui()->commandsRegistrationBuyPushButton,
            &QPushButton::clicked,
            this,
            &RegisterCommandsTabController::onBuyButtonPressed);

    // Кнопка аннулирования чека
    connect(ui()->commandsRegisterCancelPushButton,
            &QPushButton::clicked,
            this,
            &RegisterCommandsTabController::onCheckCancel);

    // Кнопка закрытия чека
    connect(ui()->commandsRegisterClosePushButton,
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

    short changeShort = ui()->commandsRegistrationChangeLineEdit->text().toUShort(&ok);

    if (changeShort > 255)
    {
        Error("Неверное значение смены.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Неверное значение смены."
        );
        return;
    }

    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Соединение с ФР отсутствует."
        );
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().sell(
            commandsTabControllerParent()->password(),
            static_cast<uint32_t>(ui()->commandsRegistrationCountDoubleSpinBox->value() * 1000),
            static_cast<uint32_t>(ui()->commandsRegistrationPriceDoubleSpinBox->value() * 100),
            static_cast<uint8_t>(changeShort),
            static_cast<uint8_t>(ui()->commandsRegistration1TaxSpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistration2TaxSpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistration3TaxSpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistration4TaxSpinBox->value()),
            "TEST"
    ))
    {
        Error("Не удалось совершить продажу.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void RegisterCommandsTabController::onBuyButtonPressed()
{
    Log("Выполняем покупку");

    QMessageBox::critical(
            parentWidget(),
            "Ошибка",
            "Еще не реализовано"
    );
//    return;

    ExcessLog("Проверка входных данных");

    bool ok = false;

    uint64_t count = static_cast<uint32_t>(ui()->commandsRegistrationCountDoubleSpinBox->value() * 100);
    uint64_t price = static_cast<uint32_t>(ui()->commandsRegistrationPriceDoubleSpinBox->value() * 100);

    short changeShort = ui()->commandsRegistrationChangeLineEdit->text().toUShort();

    if (changeShort > 255)
    {
        Error("Неверное значение смены.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Неверное значение смены."
        );
        return;
    }

    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Соединение с ФР отсутствует."
        );
        return;
    }



    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().buy(
            commandsTabControllerParent()->password(),
            count,
            price,
            static_cast<uint8_t>(changeShort),
            ui()->commandsRegistration1TaxSpinBox->value(),
            ui()->commandsRegistration2TaxSpinBox->value(),
            ui()->commandsRegistration3TaxSpinBox->value(),
            ui()->commandsRegistration4TaxSpinBox->value(),
            "TEST"
    ))
    {
        Error("Не удалось совершить продажу.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void RegisterCommandsTabController::onSellReturnButtonPressed()
{
    QMessageBox::critical(
            parentWidget(),
            "Ошибка",
            "Еще не реализовано"
    );
}

void RegisterCommandsTabController::onBuyReturnButtonPressed()
{
    QMessageBox::critical(
            parentWidget(),
            "Ошибка",
            "Еще не реализовано"
    );
}

void RegisterCommandsTabController::onCheckClose()
{
    Log("Выполняем закрытие чека.");
    ExcessLog("Проверка налиия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Соединение с ФР отсутствует."
        );
        return;
    }

    try
    {
        ExcessLog("Запрос на ФР.");
        DriverHolder::driver().closeCheck(
                commandsTabControllerParent()->password(),
                TypeConverters::toUint64(ui()->commandsRegistration1MoneyLineEdit->text()),
                TypeConverters::toUint64(ui()->commandsRegistration2MoneyLineEdit->text()),
                TypeConverters::toUint64(ui()->commandsRegistration3MoneyLineEdit->text()),
                TypeConverters::toUint64(ui()->commandsRegistration4MoneyLineEdit->text()),
                0,
                ui()->commandsRegistration1TaxSpinBox->value(),
                ui()->commandsRegistration2TaxSpinBox->value(),
                ui()->commandsRegistration3TaxSpinBox->value(),
                ui()->commandsRegistration4TaxSpinBox->value(),
                "TEST"
        );

        ExcessLog("Установка статуса.");
        commandsTabControllerParent()->setLastStatus();
    }
    catch (ConvertException& e)
    {
        QMessageBox::critical(
                parentWidget(),
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
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                "Соединение с ФР отсутствует."
        );
        return;
    }

    try
    {
        ExcessLog("Запрос на ФР.");
        DriverHolder::driver().cancelCheck(
                commandsTabControllerParent()->password()
        );

        ExcessLog("Установка статуса.");
        commandsTabControllerParent()->setLastStatus();
    }
    catch (ConvertException& e)
    {
        QMessageBox::critical(
                parentWidget(),
                "Ошибка",
                QString::fromStdString(
                        "Неверное значение одного из полей. \n" +
                        std::string(e.what())
                )
        );
        return;
    }
}

CommandsTabController *RegisterCommandsTabController::commandsTabControllerParent() const
{
    return (CommandsTabController*) parentController();
}

