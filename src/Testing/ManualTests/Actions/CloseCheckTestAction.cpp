//
// Created by megaxela on 27.02.17.
//

#include <include/Tools/Codecs.h>
#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/CloseCheckTestAction.h"

REGISTER_ACTION(CloseCheckTestAction)

CloseCheckTestAction::CloseCheckTestAction() :
    AbstractTestAction("Закрытие чека",
                       "Метод для закрытия чека",
                       {{"Auto", false},
                        {"Password", (uint32_t) 30},
                        {"Cash payment", (uint64_t) 0},
                        {"Card payment №1", (uint64_t) 0},
                        {"Card payment №2", (uint64_t) 0},
                        {"Card payment №3", (uint64_t) 0},
                        {"Discount", (uint16_t) 0},
                        {"1 tax", (uint8_t) 0},
                        {"2 tax", (uint8_t) 0},
                        {"3 tax", (uint8_t) 0},
                        {"4 tax", (uint8_t) 0},
                        {"String", std::string("")}},
                       {CATEGORY_ACTIONS})
{

}

CloseCheckTestAction::~CloseCheckTestAction()
{

}

bool CloseCheckTestAction::execute()
{
    environment()->driver()->closeCheck(
            getValue("Password").toUInt32(),
            getValue("Cash payment").toUInt64(),
            getValue("Card payment №1").toUInt64(),
            getValue("Card payment №2").toUInt64(),
            getValue("Card payment №3").toUInt64(),
            getValue("Discount").toUInt16(),
            getValue("1 tax").toUInt8(),
            getValue("2 tax").toUInt8(),
            getValue("3 tax").toUInt8(),
            getValue("4 tax").toUInt8(),
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    QByteArray(
                            getValue("String").toString().c_str()
                    )
            ).toStdString()
    );

    return true;
}

TestActionPtr CloseCheckTestAction::createAction() const
{
    return std::make_shared<CloseCheckTestAction>();
}
