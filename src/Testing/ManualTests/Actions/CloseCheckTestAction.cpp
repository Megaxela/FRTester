//
// Created by megaxela on 27.02.17.
//

#include "include/Testing/ManualTests/Actions/CloseCheckTestAction.h"


bool CloseCheckTestAction::execute()
{
//    environment()->driver()->closeCheck(
//            getValue("Пароль").toUInt32(),
//            getValue("Оплата наличными").toUInt64(),
//            getValue("Оплата картой №1").toUInt64(),
//            getValue("Оплата картой №2").toUInt64(),
//            getValue("Оплата картой №3").toUInt64(),
//            getValue("Скидка").toUInt16(),
//            getValue("1 налог").toUInt8()
//    )
}

TestActionPtr CloseCheckTestAction::createAction() const
{
    return std::make_shared<CloseCheckTestAction>();
}
