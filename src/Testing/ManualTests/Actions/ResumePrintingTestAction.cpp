//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/ResumePrintingTestAction.h"

REGISTER_ACTION(ResumePrintingTestAction)

ResumePrintingTestAction::ResumePrintingTestAction() :
    AbstractTestAction("Продожение печати",
                        "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

ResumePrintingTestAction::~ResumePrintingTestAction()
{

}

bool ResumePrintingTestAction::execute()
{
    environment()->driver()->resumePrinting(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr ResumePrintingTestAction::createAction() const
{
    return std::make_shared<ResumePrintingTestAction>();
}
