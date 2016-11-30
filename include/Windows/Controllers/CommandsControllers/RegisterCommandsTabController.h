//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H
#define FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <ui_mainwindow.h>
#include <Windows/Controllers/CommandsTabController.h>

class RegisterCommandsTabController : public AbstractTabController
{
public:
    RegisterCommandsTabController (Ui::MainWindow *ptr,
                                   QWidget* parent,
                                   AbstractTabController* parentController=nullptr);

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    void onSellButtonPressed();

    void onBuyButtonPressed();

    void onSellReturnButtonPressed();

    void onBuyReturnButtonPressed();

    void onCheckClose();

    void onCheckCancel();

private:
    Ui::MainWindow* m_ui;

    QWidget* m_parent;

    CommandsTabController* m_parentController;
};


#endif //FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H
