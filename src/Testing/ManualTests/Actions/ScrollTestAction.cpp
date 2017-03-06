//
// Created by megaxela on 27.02.17.
//

#include "include/Testing/ManualTests/Actions/ScrollTestAction.h"

ScrollTestAction::ScrollTestAction() :
    AbstractTestAction("Прокрутка ленты",
                        "",
                       {{"Пароль", (uint32_t) 30},
                        {"Количество строк", (uint8_t) 8}},
                       {CATEGORY_ACTIONS})
{

}

ScrollTestAction::~ScrollTestAction()
{

}

bool ScrollTestAction::execute()
{
    environment()->driver()->scrolling(
            getValue("Пароль").toUInt32(),
            1,
            getValue("Количество строк").toUInt8()
    );

    return true;
}

TestActionPtr ScrollTestAction::createAction() const
{
    return std::make_shared<ScrollTestAction>();
}
