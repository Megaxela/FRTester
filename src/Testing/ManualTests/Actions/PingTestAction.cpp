//
// Created by megaxela on 27.02.17.
//


#include <include/Testing/ManualTests/Actions/PingTestAction.h>

PingTestAction::PingTestAction()
{

}

PingTestAction::~PingTestAction()
{

}

bool PingTestAction::execute()
{
    return false;
}

std::shared_ptr <AbstractTestAction> PingTestAction::createAction() const
{
    return std::shared_ptr<AbstractTestAction>();
}
