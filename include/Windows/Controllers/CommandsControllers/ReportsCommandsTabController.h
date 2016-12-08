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
                                 QWidget* parent);

    CommandsTabController* commandsTabControllerParent() const;

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    void onZReportPressed();

    void onChangeOpenPressed();
};


#endif //FRTESTER_REPORTSCOMMANDSTABCONTROLLER_H
