//
// Created by megaxela on 27.02.17.
//

#include "include/Testing/ManualTests/Actions/ResumePrintingTestAction.h"

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
