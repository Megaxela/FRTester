//
// Created by megaxela on 27.02.17.
//

#include <include/Tools/Codecs.h>
#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/CloseCheckTestAction.h"

REGISTER_ACTION(CloseCheckTestAction);

CloseCheckTestAction::CloseCheckTestAction() :
    AbstractTestAction("Закрытие чека",
                       "Метод для закрытия чека",
                       {{"Автоматически", false},
                        {"Пароль", (uint32_t) 30},
                        {"Оплата наличными", (uint64_t) 0},
                        {"Оплата картой №1", (uint64_t) 0},
                        {"Оплата картой №2", (uint64_t) 0},
                        {"Оплата картой №3", (uint64_t) 0},
                        {"Скидка", (uint16_t) 0},
                        {"1 налог", (uint8_t) 0},
                        {"2 налог", (uint8_t) 0},
                        {"3 налог", (uint8_t) 0},
                        {"4 налог", (uint8_t) 0},
                        {"Строка для печати", ""}},
                       {CATEGORY_ACTIONS})
{

}


bool CloseCheckTestAction::execute()
{
    environment()->driver()->closeCheck(
            getValue("Пароль").toUInt32(),
            getValue("Оплата наличными").toUInt64(),
            getValue("Оплата картой №1").toUInt64(),
            getValue("Оплата картой №2").toUInt64(),
            getValue("Оплата картой №3").toUInt64(),
            getValue("Скидка").toUInt16(),
            getValue("1 налог").toUInt8(),
            getValue("2 налог").toUInt8(),
            getValue("3 налог").toUInt8(),
            getValue("4 налог").toUInt8(),
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    QByteArray(
                            getValue("Строка для печати").toString().c_str()
                    )
            ).toStdString()
    );
}

TestActionPtr CloseCheckTestAction::createAction() const
{
    return std::make_shared<CloseCheckTestAction>();
}
