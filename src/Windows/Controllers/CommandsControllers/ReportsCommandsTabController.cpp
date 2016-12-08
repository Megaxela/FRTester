//
// Created by megaxela on 21.11.16.
//

#include <Tools/Logger.h>
#include <include/DriverHolder.h>
#include <QtWidgets/QMessageBox>
#include "include/Windows/Controllers/CommandsControllers/ReportsCommandsTabController.h"

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
}

void ReportsCommandsTabController::configureWidgets()
{

}

void ReportsCommandsTabController::onZReportPressed()
{
    // todo: Перенести все проверки в отдельный модуль
    Log("Формируем Z отчет. (Ответ с гашением)");

    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(parentWidget(), "Ошибка", "Соединение с ФР отсутствует.");
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

    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение отсутствует.");
        QMessageBox::critical(m_parent, "Ошибка", "Соединение с ФР отсутствует.");
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
