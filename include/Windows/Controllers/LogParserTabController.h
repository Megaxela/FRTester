//
// Created by megaxela on 03.02.17.
//

#ifndef FRTESTER_LOGPARSERTABCONTROLLER_H
#define FRTESTER_LOGPARSERTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>

class LogParserTabController : public AbstractTabController
{
public:
    LogParserTabController(Ui::MainWindow* ptr,
                           QWidget* parent);


    ~LogParserTabController();

protected:
    void setupConnections() override;

    void configureWidgets() override;
};


#endif //FRTESTER_LOGPARSERTABCONTROLLER_H
