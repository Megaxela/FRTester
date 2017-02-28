//
// Created by megaxela on 27.02.17.
//

#include "include/Testing/ManualTests/Actions/RebootTestAction.h"

RebootTestAction::RebootTestAction()
{

}

RebootTestAction::~RebootTestAction()
{

}

bool RebootTestAction::execute()
{
    return false;
}

TestActionPtr RebootTestAction::createAction() const
{
    return TestActionPtr();
}
