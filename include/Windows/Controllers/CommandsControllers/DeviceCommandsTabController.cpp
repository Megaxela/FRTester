//
// Created by megaxela on 10.01.17.
//

#include "DeviceCommandsTabController.h"
#include "DeviceMoneyCommandsTabController.h"
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
}

void DeviceCommandsTabController::setupConnections()
{

}

void DeviceCommandsTabController::configureWidgets()
{

}
