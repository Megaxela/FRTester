//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/CutCheckTestAction.h"

REGISTER_ACTION(CutCheckTestAction);

CutCheckTestAction::CutCheckTestAction() :
    AbstractTestAction("Отрезка чека",
                       "",
                       {{"Пароль", (uint32_t) 30},
                        {"Полная отрезка", false}},
                       {CATEGORY_ACTIONS})
{

}

CutCheckTestAction::~CutCheckTestAction()
{

}

bool CutCheckTestAction::execute()
{
    environment()->driver()->cutCheck(
            getValue("Пароль").toUInt32(),
            (uint8_t) getValue("Полная отрезка").toBoolean()
    );

    return true;
}

TestActionPtr CutCheckTestAction::createAction() const
{
    return std::make_shared<CutCheckTestAction>();
}
