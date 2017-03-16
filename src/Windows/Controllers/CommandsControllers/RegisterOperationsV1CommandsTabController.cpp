//
// Created by megaxela on 21.11.16.
//

#include <Tools/Logger.h>
#include <include/DriverHolder.h>
#include <QtWidgets/QMessageBox>
#include <include/Tools/TypeConverters.h>
#include "include/Windows/Controllers/CommandsControllers/RegisterOperationsV1CommandsTabController.h"

RegisterOperationsV1CommandsTabController::RegisterOperationsV1CommandsTabController(Ui::MainWindow *ptr,
                                                             QWidget *parent) :
    AbstractTabController(ptr, parent, nullptr)
{

}

void RegisterOperationsV1CommandsTabController::setupConnections()
{
    // Кнопка покупки
    connect(ui()->commandsRegistrationOperationV1SellPushButton,
            &QPushButton::clicked,
            this,
            &RegisterOperationsV1CommandsTabController::onSellButtonPressed);

    // Кнопка продажи
    connect(ui()->commandsRegistrationOperationV1BuyPushButton,
            &QPushButton::clicked,
            this,
            &RegisterOperationsV1CommandsTabController::onBuyButtonPressed);

    // Кнопка аннулирования чека
    connect(ui()->commandsRegistrationOperationV1CancelPushButton,
            &QPushButton::clicked,
            this,
            &RegisterOperationsV1CommandsTabController::onCheckCancel);

    // Кнопка закрытия чека
    connect(ui()->commandsRegistrationOperationV1ClosePushButton,
            &QPushButton::clicked,
            this,
            &RegisterOperationsV1CommandsTabController::onCheckClose);
}

void RegisterOperationsV1CommandsTabController::configureWidgets()
{

}

void RegisterOperationsV1CommandsTabController::onSellButtonPressed()
{
    Log("Выполняем продажу");

    ExcessLog("Проверка входных данных");

    bool ok = false;

    short changeShort = ui()->commandsRegistrationOperationV1ChangeLineEdit->text().toUShort(&ok);

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
            static_cast<uint32_t>(ui()->commandsRegistrationOperationV1CountDoubleSpinBox->value() * 1000),
            static_cast<uint32_t>(ui()->commandsRegistrationOperationV1PriceDoubleSpinBox->value() * 100),
            static_cast<uint8_t>(changeShort),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax1SpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax2SpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax3SpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax4SpinBox->value()),
            "TEST"
    ))
    {
        Error("Не удалось совершить продажу.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void RegisterOperationsV1CommandsTabController::onBuyButtonPressed()
{
    Log("Выполняем покупку");

    QMessageBox::critical(
            parentWidget(),
            "Ошибка",
            "Еще не реализовано"
    );
//    return;

    ExcessLog("Проверка входных данных");


    uint64_t count = static_cast<uint32_t>(ui()->commandsRegistrationOperationV1CountDoubleSpinBox->value() * 100);
    uint64_t price = static_cast<uint32_t>(ui()->commandsRegistrationOperationV1PriceDoubleSpinBox->value() * 100);

    short changeShort = ui()->commandsRegistrationOperationV1ChangeLineEdit->text().toUShort();

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
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax1SpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax2SpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax3SpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV1Tax4SpinBox->value()),
            "TEST"
    ))
    {
        Error("Не удалось совершить продажу.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void RegisterOperationsV1CommandsTabController::onSellReturnButtonPressed()
{
    QMessageBox::critical(
            parentWidget(),
            "Ошибка",
            "Еще не реализовано"
    );
}

void RegisterOperationsV1CommandsTabController::onBuyReturnButtonPressed()
{
    QMessageBox::critical(
            parentWidget(),
            "Ошибка",
            "Еще не реализовано"
    );
}

void RegisterOperationsV1CommandsTabController::onCheckClose()
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
                (uint64_t) ui()->commandsRegistrationOperationV1Money1DoubleSpinBox->value() * 100,
                (uint64_t) ui()->commandsRegistrationOperationV1Money2DoubleSpinBox->value() * 100,
                (uint64_t) ui()->commandsRegistrationOperationV1Money3DoubleSpinBox->value() * 100,
                (uint64_t) ui()->commandsRegistrationOperationV1Money4DoubleSpinBox->value() * 100,
                0,
                ui()->commandsRegistrationOperationV1Tax1SpinBox->value(),
                ui()->commandsRegistrationOperationV1Tax2SpinBox->value(),
                ui()->commandsRegistrationOperationV1Tax3SpinBox->value(),
                ui()->commandsRegistrationOperationV1Tax4SpinBox->value(),
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

void RegisterOperationsV1CommandsTabController::onCheckCancel()
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

CommandsTabController *RegisterOperationsV1CommandsTabController::commandsTabControllerParent() const
{
    return (CommandsTabController*) parentController()->parentController();
}

