//
// Created by megaxela on 13.12.16.
//

#include <Windows/Controllers/CommandsControllers/FNOFDCommandsTabController.h>
#include <Windows/Controllers/FNStateCommandsTabController.h>
#include "Windows/Controllers/CommandsControllers/FNCommandsTabController.h"
#include "ui_mainwindow.h"

FNCommandsTabController::FNCommandsTabController(Ui::MainWindow *ptr,
                                                 QWidget *parent,
                                                 QTabWidget *tabWidget)
        : AbstractTabController(ptr, parent, tabWidget)
{
    addTabController(
            ui()->commandsFNOFDTab,
            new FNOFDCommandsTabController(
                    ptr,
                    parent
            )
    );

    addTabController(
            ui()->commandsFNStateTab,
            new FNStateCommandsTabController(
                    ptr,
                    parent
            )
    );
}

void FNCommandsTabController::setupConnections()
{

}

void FNCommandsTabController::configureWidgets()
{

}
