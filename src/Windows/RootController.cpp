//
// Created by megaxela on 08.12.16.
//

#include <include/Windows/Controllers/SettingsController.h>
#include <include/Windows/Controllers/TestControllers/UnitTestsController.h>
#include <include/Windows/Controllers/CommandsTabController.h>
#include <include/Windows/Controllers/ConnectControllers/ConnectCOMTabController.h>
#include <include/Windows/Controllers/ConnectTabController.h>
#include <include/Windows/Controllers/TablesTabController.h>
#include <include/Testing/SettingsSystem.h>
#include <include/Tools/Logger.h>
#include "include/Windows/Controllers/RootController.h"

RootController::RootController(Ui::MainWindow *ptr,
                               QWidget *parent,
                               QTabWidget* tabWidget) :
        AbstractTabController(ptr, parent, tabWidget)
{
    addTabController(
            ui()->connectionTab,
            new ConnectTabController(
                    ui(),
                    parentWidget(),
                    ui()->connectionTabWidget
            )
    );
    addTabController(
            ui()->tablesTab,
            new TablesTabController(
                    ui(),
                    parentWidget()
            )
    );
    addTabController(
            ui()->commandsTab,
            new CommandsTabController(
                    ui(),
                    parentWidget(),
                    ui()->commandsTabWidget
            )
    );
    addTabController(
            ui()->unitTestsTab,
            new UnitTestsController(
                    ui(),
                    parentWidget()
            )
    );
    addTabController(
            ui()->settingsTab,
            new SettingsController(
                    ui(),
                    parentWidget()
            )
    );
}

RootController::~RootController()
{

}

void RootController::setupConnections()
{
    // Соединение для контроля сохранений
    connect(tabWidget(),
            &QTabWidget::currentChanged,
            this,
            &RootController::onTabChanged);
}

void RootController::configureWidgets()
{
    try
    {
        tabWidget()->setCurrentIndex(
                std::stoi(
                        SettingsSystem::instance()
                                .getValue(
                                        SettingsSystem::GlobalTabSelected
                                )
                )
        );
    }
    catch (std::invalid_argument e)
    {
        Error("Can't get global tab current index.");
    }
}

void RootController::onTabChanged(int index)
{
    SettingsSystem::instance()
            .setValue(
                    SettingsSystem::GlobalTabSelected,
                    std::to_string(index)
            );
}

