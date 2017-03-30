//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/PrintClichesTestAction.h"

REGISTER_ACTION(PrintClichesTestAction)

PrintClichesTestAction::PrintClichesTestAction() :
    AbstractTestAction("Печать клише",
                        "",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

PrintClichesTestAction::~PrintClichesTestAction()
{

}

bool PrintClichesTestAction::execute()
{
    environment()->driver()->printCliches(
            getValue("Password").toUInt32()
    );

    return true;
}

TestActionPtr PrintClichesTestAction::createAction() const
{
    return std::make_shared<PrintClichesTestAction>();
}
