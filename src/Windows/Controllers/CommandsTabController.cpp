//
// Created by megaxela on 17.11.16.
//

#include <iostream>
#include <QtWidgets/QMessageBox>
#include <Tools/Logger.h>
#include <FRDriver.h>
#include <DriverHolder.h>
#include <Windows/Controllers/CommandsControllers/ReportsCommandsTabController.h>
#include <Windows/Controllers/CommandsControllers/RegisterCommandsTabController.h>
#include <include/Windows/Controllers/CommandsControllers/PrintCommandsTabController.h>
#include "Windows/Controllers/CommandsControllers/StateCommandsTabController.h"
#include "Windows/Controllers/CommandsTabController.h"
#include "ui_mainwindow.h"

CommandsTabController::CommandsTabController(Ui::MainWindow *ptr,
                                             QWidget* parent,
                                             QTabWidget* tabWidget) :
        AbstractTabController(ptr, parent, tabWidget)
{
    addTabController(
            ui()->commandsStatesTab,
            new StateCommandsTabController(
                    ptr,
                    parent
            )
    );
    addTabController(
            ui()->commandsReportsTab,
            new ReportsCommandsTabController(
                    ptr,
                    parent
            )
    );
    addTabController(
            ui()->commandsRegistrationTab,
            new RegisterCommandsTabController(
                    ptr,
                    parent
            )
    );
    addTabController(
            ui()->commandsPrintTab,
            new PrintCommandsTabController(
                    ptr,
                    parent,
                    ui()->commandsPrintTabWidget
            )
    );
}

CommandsTabController::~CommandsTabController()
{

}

void CommandsTabController::setupConnections()
{
    connect(m_ui->commandsTabWidget,
            &QTabWidget::currentChanged,
            this,
            &CommandsTabController::onCurrentTabChanged);
}

void CommandsTabController::configureWidgets()
{

}

uint32_t CommandsTabController::password() const
{
    bool ok = false;
    uint32_t result = m_ui->commandsPasswordLineEdit->text().toUInt(&ok);

    if (!ok)
    {
        QMessageBox::critical(m_parent, "Ошибка", "Неверное значение пароля заданного в поле \"пароль\". Использован пароль 30.");
        Log("Был указан неверный пароль. Используется пароль 30.");
        return 30;
    }

    Log("Используется пароль " + std::to_string(result) + ".");
    return result;
}

void CommandsTabController::setStatus(FRDriver::ErrorCode code, const QString &status)
{
    QString text = "#" + QString::number((int) code) + " " + status;
    m_ui->commandsResultLineEdit->setText(text);
    Log("Результат: " + text.toStdString());
}

void CommandsTabController::setLastStatus()
{
    setStatus(
            DriverHolder::driver().getLastError(),
            QString::fromStdString(
                    FRDriver::Converters::errorToString(
                            (int) DriverHolder::driver().getLastError()
                    )
            )
    );
}

void CommandsTabController::onCurrentTabChanged(int)
{
    QWidget* currentTab = m_ui->commandsTabWidget->currentWidget();

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