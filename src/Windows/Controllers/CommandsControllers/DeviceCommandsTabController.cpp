//
// Created by megaxela on 10.01.17.
//

#include "Windows/Controllers/CommandsControllers/DeviceCommandsTabController.h"
#include "Windows/Controllers/CommandsControllers/DeviceMoneyCommandsTabController.h"
#include "Windows/Controllers/CommandsControllers/DeviceActionsCommandsTabController.h"
#include "Windows/Controllers/CommandsControllers/DeviceOperationsCommandsTabController.h"
#include <ui_mainwindow.h>

DeviceCommandsTabController::DeviceCommandsTabController(Ui::MainWindow *ptr, QWidget *parent, QTabWidget *table)
        : AbstractTabController(ptr, parent, table)
{
    addTabController(
            ui()->commandsDeviceMoneyTab,
            new DeviceMoneyCommandsTabController(
                    ptr,
                    parent
            )
    );

    addTabController(
            ui()->commandsDeviceOperationsTab,
            new DeviceOperationsCommandsTabController(
                    ptr,
                    parent
            )
    );

    addTabController(
            ui()->commandsDeviceActionsTab,
            new DeviceActionsCommandsTabController(
                    ptr,
                    parent
            )
    );
}

void DeviceCommandsTabController::setupConnections()
{

}

void DeviceCommandsTabController::configureWidgets()
{

}
