//
// Created by megaxela on 17.11.16.
//

#include "include/Windows/MainWindow.h"
#include "ui_mainwindow.h"

#include "include/Windows/Controllers/CommandsTabController.h"
#include "include/Windows/Controllers/TestControllers/UnitTestsController.h"
#include "include/Windows/Controllers/ConnectTabController.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    addTabController(new ConnectTabController(m_ui, this));
    addTabController(new CommandsTabController(m_ui, this));
    addTabController(new UnitTestsController(m_ui, this));
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

void MainWindow::addTabController(AbstractTabController *controller)
{
    controller->init();
    m_tabControllers.push_back(controller);
}
