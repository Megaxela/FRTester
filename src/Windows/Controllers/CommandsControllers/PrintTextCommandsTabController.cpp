//
// Created by megaxela on 07.12.16.
//

#include <include/Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.h>
#include "include/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.h"
#include "include/Windows/Controllers/CommandsTabController.h"
#include "ui_mainwindow.h"
#include <QTabWidget>

PrintTextCommandsTabController::PrintTextCommandsTabController(Ui::MainWindow *ptr,
                                                               QWidget *parent,
                                                               QTabWidget* tabWidget) :
    AbstractTabController(ptr, parent, tabWidget)
{
    addTabController(
            ui()->commandsPrintTextOperationsTabWidget,
            new PrintTextOperationsCommandsTabController(
                    ptr,
                    parent
            )
    );
}

PrintTextCommandsTabController::~PrintTextCommandsTabController()
{

}

void PrintTextCommandsTabController::setupConnections()
{

}

void PrintTextCommandsTabController::configureWidgets()
{

}
