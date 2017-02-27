//
// Created by megaxela on 17.11.16.
//

#include <include/Tools/Logger.h>
#include <include/Windows/Controllers/RootController.h>
#include <include/Testing/TestCore.h>
#include "include/Windows/MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_rootController(nullptr)
{
    m_ui->setupUi(this);

    m_rootController = new RootController(m_ui, this, m_ui->mainTabWidget);
    m_rootController->init();

    TestCore::instance().environment()->tools()->setParentWidget(this);
}

MainWindow::~MainWindow()
{
    delete m_rootController;
}