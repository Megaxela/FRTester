//
// Created by megaxela on 17.11.16.
//

#include "Windows/Controllers/AbstractTabController.h"
#include <QDebug>
#include <iostream>
#include <include/Tools/Logger.h>
#include <include/Tools/SystemTools.h>
#include <include/Testing/SettingsSystem.h>

AbstractTabController::AbstractTabController(Ui::MainWindow* ptr,
                                             QWidget* parent,
                                             QTabWidget* tabWidget) :
    m_tabControllers(),
    m_previousTab(nullptr),
    m_ui(ptr),
    m_parent(parent),
    m_tabWidget(tabWidget),
    m_controllerParent(nullptr)
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

    // Если этот контроллер управляет tabWidget - то
    // устанавливаем активную вкладку.
    if (tabWidget() != nullptr)
    {
        auto name = SystemTools::getTypeName(*this);

        try
        {
            auto index = std::stoi(
                    SettingsSystem::instance()
                            .getValue(
                                    "active_tab_(" + name + ")"
                            )
            );

            tabWidget()->setCurrentIndex(index);

            m_previousTab = tabWidget()->currentWidget();

            auto result = m_tabControllers.find(tabWidget()->currentWidget());

            if (result != m_tabControllers.end())
            {
                result.value()->tabSelected();
            }
        }
        catch (std::invalid_argument e)
        {
            Error("Can't get saved tab for " + name + " controller.");
        }
    }
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

    if (tabWidget() != nullptr)
    {
        auto name = SystemTools::getTypeName(*this);

        SettingsSystem::instance()
                .setValue(
                        "active_tab_(" + name + ")",
                        std::to_string(tabWidget()->currentIndex())
                );
    }

    m_previousTab = currentTab;
}

void AbstractTabController::addTabController(QWidget *tab, AbstractTabController *controller)
{
    m_tabControllers[tab] = controller;
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
