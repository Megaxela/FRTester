//
// Created by megaxela on 13.03.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/WaitForPrintingFinishTestAction.h"

REGISTER_ACTION(WaitForPrintingFinishTestAction)

WaitForPrintingFinishTestAction::WaitForPrintingFinishTestAction() :
    AbstractTestAction("Ожидание окончание печати чека",
                       "",
                       {{"Password", (uint32_t) 30},
                        {"Timeout", (uint32_t) 10000}},
                       {CATEGORY_ADDITIONAL})
{

}

WaitForPrintingFinishTestAction::~WaitForPrintingFinishTestAction()
{

}

bool WaitForPrintingFinishTestAction::execute()
{
    return environment()->tools()->waitForPrintingFinished(
            getValue("Password").toUInt32(),
            getValue("Timeout").toUInt32()
    );
}

TestActionPtr WaitForPrintingFinishTestAction::createAction() const
{
    return std::make_shared<WaitForPrintingFinishTestAction>();
}
