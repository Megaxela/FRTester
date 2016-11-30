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
#include "Windows/Controllers/CommandsControllers/StateCommandsTabController.h"
#include "Windows/Controllers/CommandsTabController.h"
#include "ui_mainwindow.h"

CommandsTabController::CommandsTabController(Ui::MainWindow *ptr, QWidget* parent) :
        AbstractTabController(),
        m_ui(ptr),
        m_parent(parent)
{
    addTabController(new StateCommandsTabController(ptr, parent, this));
    addTabController(new ReportsCommandsTabController(ptr, parent, this));
    addTabController(new RegisterCommandsTabController(ptr, parent, this));
}

CommandsTabController::~CommandsTabController()
{

}

void CommandsTabController::setupConnections()
{

}

void CommandsTabController::configureWidgets()
{

}

void CommandsTabController::addTabController(AbstractTabController *el)
{
    m_tabControllers.push_back(el);
    el->init();
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
                    FRDriver::Converters::errorToString(DriverHolder::driver().getLastError())
            )
    );
}
