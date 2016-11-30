//
// Created by megaxela on 17.11.16.
//

#include "Windows/Controllers/AbstractTabController.h"
#include <QDebug>
#include <iostream>

AbstractTabController::AbstractTabController()
{

}

AbstractTabController::~AbstractTabController()
{

}

void AbstractTabController::tabSelected()
{

}

void AbstractTabController::init()
{
    configureWidgets();
    setupConnections();
}
