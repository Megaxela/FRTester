//
// Created by megaxela on 17.11.16.
//

#include "Windows/Controllers/AbstractTabController.h"
#include <QDebug>
#include <iostream>

AbstractTabController::AbstractTabController(Ui::MainWindow* ptr,
                                             QWidget* parent,
                                             QTabWidget* tabWidget) :
    m_ui(ptr),
    m_parent(parent),
    m_previousTab(nullptr),
    m_tabWidget(tabWidget)
{

}

AbstractTabController::~AbstractTabController()
{
    for (auto el : m_tabControllers)
    {
        delete el;
    }

    m_tabControllers.clear();
}

void AbstractTabController::tabSelected()
{

}

void AbstractTabController::init()
{
    configureWidgets();
    setupLocalConnections();
    setupConnections();
}

void AbstractTabController::tabLeaved()
{

}

void AbstractTabController::setupLocalConnections()
{
    if (m_tabWidget)
    {
        connect(m_tabWidget,
                &QTabWidget::currentChanged,
                this,
                &AbstractTabController::onCurrentTabChanged);
    }
}

void AbstractTabController::onCurrentTabChanged(int)
{
    QWidget* currentTab = m_tabWidget->currentWidget();

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

void AbstractTabController::addTabController(QWidget *widget, AbstractTabController *controller)
{
    m_tabControllers[widget] = controller;
    controller->setParentController(this);
    controller->init();
}

void AbstractTabController::setParentController(AbstractTabController* parent)
{
    m_controllerParent = parent;
}

AbstractTabController *AbstractTabController::parentController() const
{
    return m_controllerParent;
}

Ui::MainWindow *AbstractTabController::ui() const
{
    return m_ui;
}

QWidget *AbstractTabController::parentWidget() const
{
    return m_parent;
}

QTabWidget *AbstractTabController::tabWidget() const
{
    return m_tabWidget;
}
