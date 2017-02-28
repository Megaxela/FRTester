//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/AbstractTestAction.h>
#include <include/Testing/ManualTests/Actions/PayoutTestAction.h>

PayoutTestAction::PayoutTestAction()
{

}

PayoutTestAction::~PayoutTestAction()
{

}

bool PayoutTestAction::execute()
{
    return false;
}

TestActionPtr PayoutTestAction::createAction() const
{
    return TestActionPtr();
}
