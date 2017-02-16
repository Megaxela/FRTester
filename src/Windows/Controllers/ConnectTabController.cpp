//
// Created by megaxela on 08.12.16.
//

#include <include/Windows/Controllers/ConnectTabController.h>
#include <include/Windows/Controllers/ConnectControllers/ConnectCOMTabController.h>
#include <include/Windows/Controllers/ConnectControllers/ConnectTCPTabController.h>
#include <include/TestDriverHolder.h>
#include <include/Implementation/DefaultProtocol.h>
#include <include/DriverHolder.h>
#include <include/Tools/Logger.h>
#include "ui_mainwindow.h"

ConnectTabController::ConnectTabController(Ui::MainWindow *ptr, QWidget *parent, QTabWidget *tabWidget)
        : AbstractTabController(ptr, parent, tabWidget)
{
    auto proto = std::make_shared<DefaultProtocol>();

    TestDriverHolder::driver().setProtocol(proto);
    DriverHolder::driver().setProtocol(proto);

    addTabController(
            ui()->connectionCOMTab,
            new ConnectCOMTabController(
                    ui(),
                    parentWidget()
            )
    );

    addTabController(
            ui()->connectionTCPTab,
            new ConnectTCPTabController(
                    ui(),
                    parentWidget()
            )
    );
}

ConnectTabController::~ConnectTabController()
{

}

void ConnectTabController::setupConnections()
{

}

void ConnectTabController::configureWidgets()
{

}

void ConnectTabController::tabSelected()
{

}
