//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/PrintClichesTestAction.h"

REGISTER_ACTION(PrintClichesTestAction);

PrintClichesTestAction::PrintClichesTestAction() :
    AbstractTestAction("Печать клише",
                        "",
                       {{"Пароль", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

PrintClichesTestAction::~PrintClichesTestAction()
{

}

bool PrintClichesTestAction::execute()
{
    environment()->driver()->printCliches(
            getValue("Пароль").toUInt32()
    );

    return true;
}

TestActionPtr PrintClichesTestAction::createAction() const
{
    return std::make_shared<PrintClichesTestAction>();
}
