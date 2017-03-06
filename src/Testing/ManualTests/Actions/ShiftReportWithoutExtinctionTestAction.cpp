//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/ShiftReportWithoutExtinctionTestAction.h"

REGISTER_ACTION(ShiftReportWithoutExtinctionTestAction);

ShiftReportWithoutExtinctionTestAction::ShiftReportWithoutExtinctionTestAction() :
    AbstractTestAction("X отчет",
                       "Отчет о закрытии смены без гашения",
                       {{"Пароль", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

ShiftReportWithoutExtinctionTestAction::~ShiftReportWithoutExtinctionTestAction()
{

}

bool ShiftReportWithoutExtinctionTestAction::execute()
{
    environment()->driver()->shiftReportWithoutExtinction(
            getValue("Пароль").toUInt32()
    );

    return true;
}

TestActionPtr ShiftReportWithoutExtinctionTestAction::createAction() const
{
    return std::make_shared<ShiftReportWithoutExtinctionTestAction>();
}
