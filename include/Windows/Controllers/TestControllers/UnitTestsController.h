//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_UNITTESTSCONTROLLER_H
#define FRTESTER_UNITTESTSCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <ui_mainwindow.h>

class UnitTestsController : public AbstractTabController
{
public:
    UnitTestsController(Ui::MainWindow* ptr, QWidget* parent);

    ~UnitTestsController() override;

protected:
    void setupConnections() override;

    void configureWidgets() override;

    void tabSelected() override;

private slots:
    void onUnitTestUpdate();

    void onTestingStarted();

    void onTestingStopped();

private:
    void updateUnitTestsSet();

    Ui::MainWindow* m_ui;

    QWidget* m_parent;

};


#endif //FRTESTER_UNITTESTSCONTROLLER_H
