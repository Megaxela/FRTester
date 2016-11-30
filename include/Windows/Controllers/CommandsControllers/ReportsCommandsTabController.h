//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_REPORTSCOMMANDSTABCONTROLLER_H
#define FRTESTER_REPORTSCOMMANDSTABCONTROLLER_H

#include <include/Windows/Controllers/AbstractTabController.h>
#include <ui_mainwindow.h>
#include <include/Windows/Controllers/CommandsTabController.h>

class ReportsCommandsTabController : public AbstractTabController
{
public:
    ReportsCommandsTabController(Ui::MainWindow* ptr,
                                 QWidget* parent,
                                 AbstractTabController* parentController);

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    void onZReportPressed();

    void onChangeOpenPressed();

private:
    Ui::MainWindow* m_ui;

    QWidget* m_parent;

    CommandsTabController* m_parentController;
};


#endif //FRTESTER_REPORTSCOMMANDSTABCONTROLLER_H
