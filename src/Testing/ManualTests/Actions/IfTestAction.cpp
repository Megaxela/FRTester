//
// Created by megaxela on 22.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/IfTestAction.h"

REGISTER_ACTION(IfTestAction)

IfTestAction::IfTestAction() :
    AbstractTestAction("Если...",
                       "Действие, выполняющее определенный набор "
                       "действий при выполнении определенного условия.",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ADDITIONAL})
{

}

IfTestAction::~IfTestAction()
{

}

bool IfTestAction::execute()
{
    environment()->logger()->log("Еще не реализовано, сорян.");
    return true;
}

std::string IfTestAction::getFullName() const
{
    return AbstractTestAction::getFullName();
}

TestActionPtr IfTestAction::createAction() const
{
    return std::make_shared<IfTestAction>();
}
