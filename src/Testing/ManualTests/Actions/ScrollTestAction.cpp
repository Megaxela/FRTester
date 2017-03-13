//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/ScrollTestAction.h"

REGISTER_ACTION(ScrollTestAction)

ScrollTestAction::ScrollTestAction() :
    AbstractTestAction("Прокрутка ленты",
                        "",
                       {{"Password", (uint32_t) 30},
                        {"Count", (uint8_t) 8}},
                       {CATEGORY_ACTIONS})
{

}

ScrollTestAction::~ScrollTestAction()
{

}

bool ScrollTestAction::execute()
{
    environment()->driver()->scrolling(
            getValue("Password").toUInt32(),
            1,
            getValue("Count").toUInt8()
    );

    return true;
}

TestActionPtr ScrollTestAction::createAction() const
{
    return std::make_shared<ScrollTestAction>();
}
