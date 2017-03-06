//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include <include/Tools/Codecs.h>
#include "include/Testing/ManualTests/Actions/ReturnBuyTestAction.h"

REGISTER_ACTION(ReturnBuyTestAction);

ReturnBuyTestAction::ReturnBuyTestAction() :
    AbstractTestAction("Возврат покупки",
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

ReturnBuyTestAction::~ReturnBuyTestAction()
{

}

bool ReturnBuyTestAction::execute()
{
    environment()->driver()->returnBuy(
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

TestActionPtr ReturnBuyTestAction::createAction() const
{
    return std::make_shared<ReturnBuyTestAction>();
}
