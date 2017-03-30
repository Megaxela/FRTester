//
// Created by megaxela on 15.03.17.
//

#ifndef FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H
#define FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>

class RegisterCommandsTabController : public AbstractTabController
{
public:
    RegisterCommandsTabController(Ui::MainWindow *ptr, QWidget *parent, QTabWidget *tabWidget);

protected:
    virtual void setupConnections();

    virtual void configureWidgets();


};


#endif //FRTESTER_REGISTERCOMMANDSTABCONTROLLER_H
