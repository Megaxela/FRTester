//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/CancelCheckTestAction.h"

REGISTER_ACTION(CancelCheckTestAction)

CancelCheckTestAction::CancelCheckTestAction() :
    AbstractTestAction("Аннулирование чека",
                       "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

CancelCheckTestAction::~CancelCheckTestAction()
{

}

bool CancelCheckTestAction::execute()
{
    environment()->driver()->cancelCheck(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr CancelCheckTestAction::createAction() const
{
    return std::make_shared<CancelCheckTestAction>();
}
