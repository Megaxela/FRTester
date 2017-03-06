//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/CutCheckTestAction.h"

REGISTER_ACTION(CutCheckTestAction);

CutCheckTestAction::CutCheckTestAction() :
    AbstractTestAction("Отрезка чека",
                       "",
                       {{"Password", (uint32_t) 30},
                        {"Full cut", false}},
                       {CATEGORY_ACTIONS})
{

}

CutCheckTestAction::~CutCheckTestAction()
{

}

bool CutCheckTestAction::execute()
{
    environment()->driver()->cutCheck(
            getValue("Password").toUInt32(),
            (uint8_t) getValue("Full cut").toBoolean()
    );

    return true;
}

TestActionPtr CutCheckTestAction::createAction() const
{
    return std::make_shared<CutCheckTestAction>();
}
