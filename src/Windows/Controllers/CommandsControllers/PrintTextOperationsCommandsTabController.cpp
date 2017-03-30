//
// Created by megaxela on 07.12.16.
//

#include <Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.h>
#include <Windows/Controllers/CommandsTabController.h>
#include <Tools/Logger.h>
#include <DriverHolder.h>
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

    // Кнопка отрезки
    connect(ui()->commandsPrintTextOperationsCutPushButton,
            &QPushButton::clicked,
            this,
            &PrintTextOperationsCommandsTabController::onCutPressed);

    // Кнопка прокрутки
    connect(ui()->commandsPrintTextOperationsScrollPaperPushButton,
            &QPushButton::clicked,
            this,
            &PrintTextOperationsCommandsTabController::onScrollPressed);
}

void PrintTextOperationsCommandsTabController::configureWidgets()
{

}

CommandsTabController *PrintTextOperationsCommandsTabController::commandsTabControllerParent() const
{
    return (CommandsTabController*) parentController()->parentController()->parentController();
}

void PrintTextOperationsCommandsTabController::onContinuePrintPressed()
{
    Log("Продолжаем печать.");
    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");

    if (!DriverHolder::driver().resumePrinting(commandsTabControllerParent()->password()))
    {
        Error("Не удалось продолжить печать.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void PrintTextOperationsCommandsTabController::onCutPressed()
{
    Log("Отрезаем ленту.");
    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");

    if (!DriverHolder::driver().cutCheck(
            commandsTabControllerParent()->password(),
            (uint8_t) ui()->commandsPrintTextOperationsCutTypeComboBox->currentIndex()
    ))
    {
        Error("Не удалось отрезать ленту.");
    }

    commandsTabControllerParent()->setLastStatus();
}

void PrintTextOperationsCommandsTabController::onScrollPressed()
{
    Log("Проскролливаем ленту.");
    if (!commandsTabControllerParent()->checkConnectionWithDevice())
    {
        Error("Соединение с ФР отсутствует.");
        return;
    }

    ExcessLog("Запрос на ФР.");

    if (!DriverHolder::driver().scrolling(
            commandsTabControllerParent()->password(),
            7, // 0b111 C++14
            (uint8_t) ui()->commandsPrintTextOperationsScrollLinesSpinBox->value()
    ))
    {
        Error("Не удалось прокрутить ленту.");
    }

    commandsTabControllerParent()->setLastStatus();
}
