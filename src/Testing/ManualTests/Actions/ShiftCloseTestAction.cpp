//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/ShiftCloseTestAction.h"

REGISTER_ACTION(ShiftCloseTestAction)

ShiftCloseTestAction::ShiftCloseTestAction() :
    AbstractTestAction("Z отчет",
                       "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

ShiftCloseTestAction::~ShiftCloseTestAction()
{

}

bool ShiftCloseTestAction::execute()
{
    environment()->driver()->shiftCloseReport(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr ShiftCloseTestAction::createAction() const
{
    return std::make_shared<ShiftCloseTestAction>();
}
