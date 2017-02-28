//
// Created by megaxela on 27.02.17.
//

#include "include/Testing/ManualTests/Actions/CancelCheckTestAction.h"

CancelCheckTestAction::CancelCheckTestAction() :
    AbstractTestAction("Аннулирование чека",
                       "",
                       {{"Пароль", (uint32_t) 30}})
{

}

CancelCheckTestAction::~CancelCheckTestAction()
{

}

bool CancelCheckTestAction::execute()
{
    environment()->driver()->cancelCheck(
            getValue("Пароль").toUInt32()
    );

    return true;
}

TestActionPtr CancelCheckTestAction::createAction() const
{
    return std::make_shared<CancelCheckTestAction>();
}
