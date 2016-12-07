//
// Created by megaxela on 07.12.16.
//

#include <include/Windows/Controllers/CommandsControllers/PrintTextOperationsCommandsTabController.h>
#include "include/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.h"
#include "include/Windows/Controllers/CommandsTabController.h"
#include "ui_mainwindow.h"
#include <QTabWidget>

PrintTextCommandsTabController::PrintTextCommandsTabController(Ui::MainWindow *ptr, QWidget *parent,
                                                               AbstractTabController *parentController) :
    AbstractTabController(),
    m_ui(ptr),
    m_parent(parent),
    m_parentController(dynamic_cast<CommandsTabController*>(parentController))
{
    addTabController(
            m_ui->commandsPrintTextOperationsTabWidget,
            new PrintTextOperationsCommandsTabController(ptr, parent, parentController)
    );
}

PrintTextCommandsTabController::~PrintTextCommandsTabController()
{
    for (auto el : m_tabControllers)
    {
        delete el;
    }

    m_tabControllers.clear();
}

void PrintTextCommandsTabController::setupConnections()
{
    connect(m_ui->commandsPrintTextTabWidget,
            &QTabWidget::currentChanged,
            this,
            &PrintTextCommandsTabController::onCurrentTabChanged);
}

void PrintTextCommandsTabController::onCurrentTabChanged(int)
{
    QWidget* currentTab = m_ui->commandsPrintTextTabWidget->currentWidget();

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

void PrintTextCommandsTabController::addTabController(QWidget *widget, AbstractTabController *el)
{
    m_tabControllers[widget] = el;
    el->init();
}

void PrintTextCommandsTabController::configureWidgets()
{

}
