//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/TaxesReportTestAction.h"

REGISTER_ACTION(TaxesReportTestAction)

TaxesReportTestAction::TaxesReportTestAction() :
    AbstractTestAction("Отчет о налогах",
                        "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

TaxesReportTestAction::~TaxesReportTestAction()
{

}

bool TaxesReportTestAction::execute()
{
    environment()->driver()->taxesReport(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr TaxesReportTestAction::createAction() const
{
    return std::make_shared<TaxesReportTestAction>();
}
