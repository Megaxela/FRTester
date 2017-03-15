//
// Created by megaxela on 15.03.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/WriteTableStringTestAction.h"

REGISTER_ACTION(WriteTableStringTestAction)

WriteTableStringTestAction::WriteTableStringTestAction() :
    AbstractTestAction("Запись строки в таблицу",
                       "",
                       {{"Password", (uint32_t) 30},
                        {"Table number", (uint8_t) 0},
                        {"Row", (uint16_t) 0},
                        {"Field", (uint8_t) 0},
                        {"Value", std::string()}},
                       {CATEGORY_ACTIONS})
{

}

WriteTableStringTestAction::~WriteTableStringTestAction()
{

}

bool WriteTableStringTestAction::execute()
{
    environment()->driver()->writeTable(
            getValue("Password").toUInt32(),
            getValue("Table number").toUInt8(),
            getValue("Row").toUInt16(),
            getValue("Field").toUInt8(),
            getValue("Value").toString()
    );

    return true;
}

TestActionPtr WriteTableStringTestAction::createAction() const
{
    return std::make_shared<WriteTableStringTestAction>();
}
