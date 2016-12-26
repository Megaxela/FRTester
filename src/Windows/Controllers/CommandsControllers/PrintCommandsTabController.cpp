//
// Created by megaxela on 07.12.16.
//

#include <include/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.h>
#include <include/Windows/Controllers/CommandsControllers/PrintTextPlainTextCommandsTabController.h>
#include "include/Windows/Controllers/CommandsControllers/PrintCommandsTabController.h"
#include "ui_mainwindow.h"
#include "include/Windows/Controllers/CommandsTabController.h"

PrintCommandsTabController::PrintCommandsTabController(Ui::MainWindow *ptr,
                                                       QWidget *parent,
                                                       QTabWidget* tabWidget) :
    AbstractTabController(ptr, parent, tabWidget)
{
    addTabController(
            ui()->commandsPrintTextPlainTextTabWidget,
            new PrintTextPlainTextCommandsTabController(
                    ptr,
                    parent
            )
    );

    addTabController(
            ui()->commandsPrintTextTab,
            new PrintTextCommandsTabController(
                    ptr,
                    parent,
                    ui()->commandsPrintTextTabWidget
            )
    );
}

PrintCommandsTabController::~PrintCommandsTabController()
{

}

void PrintCommandsTabController::setupConnections()
{

}

void PrintCommandsTabController::configureWidgets()
{

}