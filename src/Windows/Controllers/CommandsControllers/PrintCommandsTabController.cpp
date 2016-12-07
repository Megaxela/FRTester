//
// Created by megaxela on 07.12.16.
//

#include <include/Windows/Controllers/CommandsControllers/PrintTextCommandsTabController.h>
#include "include/Windows/Controllers/CommandsControllers/PrintCommandsTabController.h"
#include "ui_mainwindow.h"
#include "include/Windows/Controllers/CommandsTabController.h"

PrintCommandsTabController::PrintCommandsTabController(Ui::MainWindow *ptr, QWidget *parent,
                                                       AbstractTabController *parentController) :
    AbstractTabController(),
    m_ui(ptr),
    m_parent(parent),
    m_parentController(dynamic_cast<CommandsTabController*>(parentController))
{
    addTabController(m_ui->commandsPrintTextTab, new PrintTextCommandsTabController(ptr, parent, parentController));
}

PrintCommandsTabController::~PrintCommandsTabController()
{
    for (auto el : m_tabControllers)
    {
        delete el;
    }

    m_tabControllers.clear();
}

void PrintCommandsTabController::setupConnections()
{
    connect(m_ui->commandsPrintTabWidget,
            &QTabWidget::currentChanged,
            this,
            &PrintCommandsTabController::onCurrentTabChanged);
}

void PrintCommandsTabController::configureWidgets()
{

}

void PrintCommandsTabController::addTabController(QWidget *widget, AbstractTabController *el)
{
    m_tabControllers[widget] = el;
    el->init();
}

void PrintCommandsTabController::onCurrentTabChanged(int)
{
    QWidget* currentTab = m_ui->commnadsPrintTabWidget->currentWidget();

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
