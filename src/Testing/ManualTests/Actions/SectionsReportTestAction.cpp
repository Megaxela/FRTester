//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/SectionsReportTestAction.h"

REGISTER_ACTION(SectionsReportTestAction)

SectionsReportTestAction::SectionsReportTestAction() :
    AbstractTestAction("Печать отчета по секциям",
                       "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

SectionsReportTestAction::~SectionsReportTestAction()
{

}

bool SectionsReportTestAction::execute()
{
    environment()->driver()->sectionsReport(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr SectionsReportTestAction::createAction() const
{
    return std::make_shared<SectionsReportTestAction>();
}
