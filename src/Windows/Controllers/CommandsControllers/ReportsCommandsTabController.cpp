//
// Created by megaxela on 21.11.16.
//

#include <Tools/Logger.h>
#include <DriverHolder.h>
#include <QMessageBox>
#include "Windows/Controllers/CommandsControllers/ReportsCommandsTabController.h"

ReportsCommandsTabController::ReportsCommandsTabController(Ui::MainWindow *ptr,
                                                           QWidget *parent) :
    AbstractTabController(ptr, parent)
{

}

void ReportsCommandsTabController::setupConnections()
{
    // Кнопка Z отчета
    connect(ui()->commandsReportsZReportPushButton,
            &QPushButton::clicked,
            this,
            &ReportsCommandsTabController::onZReportPressed);

    // Отрытие смены
    connect(ui()->commandsReportsOpenChangePushButton,
            &QPushButton::clicked,
            this,
            &ReportsCommandsTabController::onChangeOpenPressed);

    // Кнопка X отчета
    connect(ui()->commandsReportsXReportPushButton,
            &QPushButton::clicked,
            this,
            &ReportsCommandsTabController::onXReportPressed);

    // Кнопка отчета по отделам
    connect(ui()->commandsReportsDepartmentReportPushButton,
            &QPushButton::clicked,
            this,
            &ReportsCommandsTabController::onDepartmentReportPressed);

    // Кнопка отчета по налогам
    connect(ui()->commandsReportsTaxReportPushButton,
            &QPushButton::clicked,
            this,
            &ReportsCommandsTabController::onTaxesReportPressed);

    // Кнопка отчета по кассирам
    connect(ui()->commandsReportsCashierReportPushButton,
            &QPushButton::clicked,
            this,
            &ReportsCommandsTabController::onCashiersReportPressed);
}

void ReportsCommandsTabController::configureWidgets()
{

}

void ReportsCommandsTabController::onZReportPressed()
{
    Log("Формируем Z отчет. (Ответ с гашением)");
    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().shiftCloseReport(
            commandsTabControllerParent()->password()
    ))
    {
        Error("Не удалось сформировать Z отчет.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void ReportsCommandsTabController::onChangeOpenPressed()
{
    Log("Открываем смену.");

    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().openShift(commandsTabControllerParent()->password()))
    {
        Error("Не удалось открыть смену.");
    }

    commandsTabControllerParent()->setLastStatus();
}

CommandsTabController *ReportsCommandsTabController::commandsTabControllerParent() const
{
    return (CommandsTabController*) parentController();
}

void ReportsCommandsTabController::onXReportPressed()
{
    Log("Печатаем отчет без гашения.");

    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().shiftReportWithoutExtinction(commandsTabControllerParent()->password()))
    {
        Error("Не удалось снять отчет без гашения.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void ReportsCommandsTabController::onDepartmentReportPressed()
{
    Log("Печатаем отчет по отделам.");

    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().sectionsReport(commandsTabControllerParent()->password()))
    {
        Error("Не удалось снять отчет по отделам.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void ReportsCommandsTabController::onTaxesReportPressed()
{
    Log("Печатаем отчет по налогам.");

    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().taxesReport(commandsTabControllerParent()->password()))
    {
        Error("Не удалось снять отчет по налогам.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void ReportsCommandsTabController::onCashiersReportPressed()
{
    Log("Печатаем отчет по кассирам.");

    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().cashierReport(commandsTabControllerParent()->password()))
    {
        Error("Не удалось снять отчет по кассирам.");
    }

    commandsTabControllerParent()->setLastStatus();
}
