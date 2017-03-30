//
// Created by megaxela on 15.03.17.
//

#include <DriverHolder.h>
#include <Tools/Codecs.h>
#include "Windows/Controllers/CommandsControllers/RegisterOperationsV2CommandsTabController.h"
#include "ui_mainwindow.h"

RegisterOperationsV2CommandsTabController::RegisterOperationsV2CommandsTabController(Ui::MainWindow *ptr,
                                                                                     QWidget *parent) :
    AbstractTabController(ptr, parent)
{

}

CommandsTabController *RegisterOperationsV2CommandsTabController::commandsTabController() const
{
    return static_cast<CommandsTabController *>(parentController()->parentController());
}

void RegisterOperationsV2CommandsTabController::setupConnections()
{
    connect(ui()->commandsRegistrationOperationV2BuyPushButton,
            &QPushButton::pressed,
            this,
            &RegisterOperationsV2CommandsTabController::onBuyButtonPressed);

    connect(ui()->commandsRegistrationOperationV2SellPushButton,
            &QPushButton::pressed,
            this,
            &RegisterOperationsV2CommandsTabController::onSellButtonPressed);

    connect(ui()->commandsRegistrationOperationV2ReturnBuyPushButton,
            &QPushButton::pressed,
            this,
            &RegisterOperationsV2CommandsTabController::onReturnBuyButtonPressed);

    connect(ui()->commandsRegistrationOperationV2ReturnSellPushButton,
            &QPushButton::pressed,
            this,
            &RegisterOperationsV2CommandsTabController::onReturnSellButtonPressed);

    connect(ui()->commandsRegistrationOperationV2CancelPushButton,
            &QPushButton::pressed,
            this,
            &RegisterOperationsV2CommandsTabController::onCheckCancelButtonPressed);

    connect(ui()->commandsRegistrationOperationV2ResumePrintingPushButton,
            &QPushButton::pressed,
            this,
            &RegisterOperationsV2CommandsTabController::onResumePrintingButtonPressed);

    connect(ui()->commandsRegistrationOperationV2ExtendedClosePushButton,
            &QPushButton::pressed,
            this,
            &RegisterOperationsV2CommandsTabController::onCheckCloseButtonPressed);
}

void RegisterOperationsV2CommandsTabController::configureWidgets()
{

}

void RegisterOperationsV2CommandsTabController::onBuyButtonPressed()
{
    performOperation(FRDriver::OperationType::Consumption);
}

void RegisterOperationsV2CommandsTabController::onSellButtonPressed()
{
    performOperation(FRDriver::OperationType::Incoming);
}

void RegisterOperationsV2CommandsTabController::onReturnBuyButtonPressed()
{
    performOperation(FRDriver::OperationType::ReturnConsumption);
}

void RegisterOperationsV2CommandsTabController::onReturnSellButtonPressed()
{
    performOperation(FRDriver::OperationType::ReturnIncoming);
}

void RegisterOperationsV2CommandsTabController::onResumePrintingButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    DriverHolder::driver().resumePrinting(
            commandsTabController()->password()
    );

    commandsTabController()->setLastStatus();
}

void RegisterOperationsV2CommandsTabController::onCheckCancelButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }

    DriverHolder::driver().cancelCheck(
            commandsTabController()->password()
    );

    commandsTabController()->setLastStatus();
}

void RegisterOperationsV2CommandsTabController::onCheckCloseButtonPressed()
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return;
    }



    commandsTabController()->setLastStatus();
}

uint64_t RegisterOperationsV2CommandsTabController::performOperation(FRDriver::OperationType operationType)
{
    if (!commandsTabController()->checkConnectionWithDevice())
    {
        return 0;
    }

    auto result = DriverHolder::driver().operationV2(
            commandsTabController()->password(),
            operationType,
            static_cast<uint64_t>(ui()->commandsRegistrationOperationV2CountDoubleSpinBox->value() * 10),
            static_cast<uint64_t>(ui()->commandsRegistrationOperationV2PriceDoubleSpinBox->value() * 10),
            static_cast<uint64_t>(ui()->commandsRegistrationOperationV2TaxValueDoubleSpinBox->value() * 10),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV2TaxSpinBox->value()),
            static_cast<uint8_t>(ui()->commandsRegistrationOperationV2DepartmentSpinBox->value()),
            0,
            0,
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    ui()->commandsRegistrationOperationV2GoodNameLineEdit->text().toUtf8()
            ).toStdString()
    );

    commandsTabController()->setLastStatus();

    return result;
}
