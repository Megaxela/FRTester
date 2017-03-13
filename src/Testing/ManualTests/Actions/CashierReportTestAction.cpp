//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/Actions/CashierReportTestAction.h>
#include <include/Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(CashierReportTestAction)

CashierReportTestAction::CashierReportTestAction() :
    AbstractTestAction("Отчет по кассирам",
                       "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

CashierReportTestAction::~CashierReportTestAction()
{

}

bool CashierReportTestAction::execute()
{
    auto pass = getValue("Password").toUInt32();

    environment()->driver()->cashierReport(pass);

    return true;
}

TestActionPtr CashierReportTestAction::createAction() const
{
    return std::shared_ptr<CashierReportTestAction>();
}
