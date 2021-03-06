//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/ShiftReportWithoutExtinctionTestAction.h"

REGISTER_ACTION(ShiftReportWithoutExtinctionTestAction)

ShiftReportWithoutExtinctionTestAction::ShiftReportWithoutExtinctionTestAction() :
    AbstractTestAction("X отчет",
                       "Отчет о закрытии смены без гашения",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

ShiftReportWithoutExtinctionTestAction::~ShiftReportWithoutExtinctionTestAction()
{

}

bool ShiftReportWithoutExtinctionTestAction::execute()
{
    environment()->driver()->shiftReportWithoutExtinction(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr ShiftReportWithoutExtinctionTestAction::createAction() const
{
    return std::make_shared<ShiftReportWithoutExtinctionTestAction>();
}
