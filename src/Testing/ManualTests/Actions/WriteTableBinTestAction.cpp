//
// Created by megaxela on 15.03.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/WriteTableBinTestAction.h"

REGISTER_ACTION(WriteTableBinTestAction)

WriteTableBinTestAction::WriteTableBinTestAction() :
        AbstractTestAction("Запись числа в таблицу",
                           "",
                           {{"Password", (uint32_t) 30},
                            {"Table number", (uint8_t) 0},
                            {"Row", (uint16_t) 0},
                            {"Field", (uint8_t) 0},
                            {"Value", (uint64_t) 0},
                            {"Size", (uint8_t) 2}},
                           {CATEGORY_ACTIONS})
{

}

WriteTableBinTestAction::~WriteTableBinTestAction()
{

}

bool WriteTableBinTestAction::execute()
{
    environment()->driver()->writeTable(
            getValue("Password").toUInt32(),
            getValue("Table number").toUInt8(),
            getValue("Row").toUInt16(),
            getValue("Field").toUInt8(),
            getValue("Value").toUInt64(),
            getValue("Size").toUInt8()
    );

    return true;
}

TestActionPtr WriteTableBinTestAction::createAction() const
{
    return TestActionPtr();
}
