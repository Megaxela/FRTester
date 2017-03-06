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
                       {{"Password", (uint32_t) 30},
                        {"Count", (uint64_t) 1000},
                        {"Price", (uint64_t) 10000},
                        {"Department", (uint8_t) 1},
                        {"1 tax", (uint8_t) 0},
                        {"2 tax", (uint8_t) 0},
                        {"3 tax", (uint8_t) 0},
                        {"4 tax", (uint8_t) 0},
                        {"String", "String для печати"}},
                       {CATEGORY_ACTIONS})
{

}

BuyTestAction::~BuyTestAction()
{

}

bool BuyTestAction::execute()
{
    environment()->driver()->buy(
            getValue("Password").toUInt32(),
            getValue("Count").toUInt64(),
            getValue("Price").toUInt64(),
            getValue("Department").toUInt8(),
            getValue("1 tax").toUInt8(),
            getValue("2 tax").toUInt8(),
            getValue("3 tax").toUInt8(),
            getValue("4 tax").toUInt8(),
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    getValue("String").toString().c_str()
            ).toStdString()
    );

    return true;
}

TestActionPtr BuyTestAction::createAction() const
{
    return std::make_shared<BuyTestAction>();
}
