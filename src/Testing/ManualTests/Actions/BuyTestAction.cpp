//
// Created by megaxela on 27.02.17.
//

#include <include/Tools/Codecs.h>
#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/BuyTestAction.h"

REGISTER_ACTION(BuyTestAction);

BuyTestAction::BuyTestAction() :
    AbstractTestAction("Покупка",
                       "",
                       {{"Пароль", (uint32_t) 30},
                        {"Количество", (uint64_t) 1000},
                        {"Цена", (uint64_t) 10000},
                        {"Отдел", (uint8_t) 1},
                        {"1 налог", (uint8_t) 0},
                        {"2 налог", (uint8_t) 0},
                        {"3 налог", (uint8_t) 0},
                        {"4 налог", (uint8_t) 0},
                        {"Строка", "Строка для печати"}},
                       {CATEGORY_ACTIONS})
{

}

BuyTestAction::~BuyTestAction()
{

}

bool BuyTestAction::execute()
{
    environment()->driver()->buy(
            getValue("Пароль").toUInt32(),
            getValue("Количество").toUInt64(),
            getValue("Цена").toUInt64(),
            getValue("Отдел").toUInt8(),
            getValue("1 налог").toUInt8(),
            getValue("2 налог").toUInt8(),
            getValue("3 налог").toUInt8(),
            getValue("4 налог").toUInt8(),
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    getValue("Строка").toString().c_str()
            ).toStdString()
    );

    return true;
}

TestActionPtr BuyTestAction::createAction() const
{
    return std::make_shared<BuyTestAction>();
}
