//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/CancelCheckTestAction.h"

REGISTER_ACTION(CancelCheckTestAction);

CancelCheckTestAction::CancelCheckTestAction() :
    AbstractTestAction("Аннулирование чека",
                       "",
                       {{"Пароль", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

CancelCheckTestAction::~CancelCheckTestAction()
{

}

bool CancelCheckTestAction::execute()
{
    environment()->driver()->cancelCheck(
            getValue("Пароль").toUInt32()
    );

    return true;
}

TestActionPtr CancelCheckTestAction::createAction() const
{
    return std::make_shared<CancelCheckTestAction>();
}
