//
// Created by megaxela on 21.11.16.
//

#include <Tools/Logger.h>
#include <include/DriverHolder.h>
#include <QtWidgets/QMessageBox>
#include "include/Windows/Controllers/CommandsControllers/ReportsCommandsTabController.h"

ReportsCommandsTabController::ReportsCommandsTabController(Ui::MainWindow *ptr,
                                                           QWidget *parent,
                                                           AbstractTabController *parentController) :
    AbstractTabController(),
    m_ui(ptr),
    m_parent(parent),
    m_parentController(dynamic_cast<CommandsTabController*>(parentController))
{

}

void ReportsCommandsTabController::setupConnections()
{
    // Кнопка Z отчета
    connect(m_ui->commandsReportsZReportPushButton,
            &QPushButton::clicked,
            this,
            &ReportsCommandsTabController::onZReportPressed);

    // Отрытие смены
    connect(m_ui->commandsReportsOpenChangePushButton,
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
        QMessageBox::critical(m_parent, "Ошибка", "Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");
    if (!DriverHolder::driver().shiftCloseReport(
            m_parentController->password()
    ))
    {
        Error("Не удалось сформировать Z отчет.");
    }

    m_parentController->setLastStatus();
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
    if (!DriverHolder::driver().openShift(m_parentController->password()))
    {
        Error("Не удалось открыть смену.");
    }

    m_parentController->setLastStatus();
}
