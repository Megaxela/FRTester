//
// Created by megaxela on 08.12.16.
//

#include <Windows/Controllers/ConnectTabController.h>
#include <Windows/Controllers/ConnectControllers/ConnectCOMTabController.h>
#include <Windows/Controllers/ConnectControllers/ConnectTCPTabController.h>
#include <TestDriverHolder.h>
#include <Implementation/DefaultProtocol.h>
#include <DriverHolder.h>
#include <Tools/Logger.h>
#include <ui_mainwindow.h>

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
