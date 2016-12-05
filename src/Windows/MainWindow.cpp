//
// Created by megaxela on 17.11.16.
//

#include <include/Tools/Logger.h>
#include <include/Windows/Controllers/SettingsController.h>
#include "include/Windows/MainWindow.h"
#include "ui_mainwindow.h"

#include "include/Windows/Controllers/CommandsTabController.h"
#include "include/Windows/Controllers/TestControllers/UnitTestsController.h"
#include "include/Windows/Controllers/ConnectTabController.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_previousTab(nullptr),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    addTabController(m_ui->connectionTab, new ConnectTabController(m_ui, this));
    addTabController(m_ui->commandsTab, new CommandsTabController(m_ui, this));
    addTabController(m_ui->unitTestsTab, new UnitTestsController(m_ui, this));
    addTabController(m_ui->settingsTab, new SettingsController(m_ui, this));

    connect(m_ui->mainTabWidget,
            &QTabWidget::currentChanged,
            this,
            &MainWindow::onCurrentTabChanged);
}

MainWindow::~MainWindow()
{
    // Removing tab controllers
    for (auto el : m_tabControllers)
    {
        delete el;
    }

    m_tabControllers.clear();
}

void MainWindow::addTabController(QWidget* tabWidget, AbstractTabController *controller)
{
    controller->init();
    m_tabControllers[tabWidget] = controller;
}

void MainWindow::onCurrentTabChanged(int)
{
    QWidget* currentTab = m_ui->mainTabWidget->currentWidget();

    if (m_previousTab != nullptr)
    {
        if (m_tabControllers.contains(m_previousTab))
        {
            m_tabControllers[m_previousTab]->tabLeaved();
        }
    }

    if (m_tabControllers.contains(currentTab))
    {
        m_tabControllers[currentTab]->tabSelected();
    }

    m_previousTab = currentTab;
}
