//
// Created by megaxela on 15.03.17.
//

#include "Windows/Controllers/CommandsControllers/RegisterCommandsTabController.h"
#include "Windows/Controllers/CommandsControllers/RegisterOperationsV1CommandsTabController.h"
#include "Windows/Controllers/CommandsControllers/RegisterOperationsV2CommandsTabController.h"

RegisterCommandsTabController::RegisterCommandsTabController(Ui::MainWindow *ptr,
                                                             QWidget *parent,
                                                             QTabWidget *tabWidget) :
        AbstractTabController(ptr,
                              parent,
                              tabWidget)
{
    addTabController(
            ui()->commandsRegistrationOperationsV1Tab,
            new RegisterOperationsV1CommandsTabController(
                    ptr,
                    parent
            )
    );

    addTabController(
            ui()->commandsRegistrationOperationsV2Tab,
            new RegisterOperationsV2CommandsTabController(
                    ptr,
                    parent
            )
    );
}

void RegisterCommandsTabController::setupConnections()
{

}

void RegisterCommandsTabController::configureWidgets()
{

}
