//
// Created by megaxela on 07.12.16.
//

#include "include/Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.h"
#include "include/Windows/Controllers/CommandsTabController.h"
#include <include/Tools/Logger.h>
#include <include/DriverHolder.h>
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>

PrintTextOperationsCommandsTabController::PrintTextOperationsCommandsTabController(Ui::MainWindow *ptr,
                                                                                   QWidget *parent) :
    AbstractTabController(ptr, parent)
{

}

PrintTextOperationsCommandsTabController::~PrintTextOperationsCommandsTabController()
{

}

void PrintTextOperationsCommandsTabController::setupConnections()
{
    // Кнопка продолжения печати
    connect(ui()->commandsPrintTextOperationsContinuePrintPushButton,
            &QPushButton::clicked,
            this,
            &PrintTextOperationsCommandsTabController::onContinuePrintPressed);
}

void PrintTextOperationsCommandsTabController::configureWidgets()
{

}

void PrintTextOperationsCommandsTabController::onContinuePrintPressed()
{
    // todo: Перенести все проверки в отдельный модуль
    Log("Продолжаем печать.");

    ExcessLog("Проверка наличия соединения");
    if (!DriverHolder::driver().checkConnection())
    {
        Error("Соединение с ФР отсутствует.");
        QMessageBox::critical(parentWidget(), "Ошибка", "Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");

    if (!DriverHolder::driver().resumePrinting(commandsTabControllerParent()->password()))
    {
        Error("Не удалось продолжить печать.");
    }

    commandsTabControllerParent()->setLastStatus();
}

CommandsTabController *PrintTextOperationsCommandsTabController::commandsTabControllerParent() const
{
    return (CommandsTabController*) parentController()->parentController()->parentController();
}
