//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include <include/Tools/Codecs.h>
#include "include/Testing/ManualTests/Actions/StandardStringPrintTestAction.h"

REGISTER_ACTION(StandardStringPrintTestAction);

StandardStringPrintTestAction::StandardStringPrintTestAction() :
    AbstractTestAction("Печать строки",
                        "",
                       {{"Пароль", (uint32_t) 30},
                        {"Строка", ""}},
                       {CATEGORY_ACTIONS})
{

}

StandardStringPrintTestAction::~StandardStringPrintTestAction()
{

}

bool StandardStringPrintTestAction::execute()
{
    environment()->driver()->standardStringPrint(
            getValue("Пароль").toUInt32(),
            1,
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    getValue("Строка").toString().c_str()
            ).toStdString()
    );

    return true;
}

TestActionPtr StandardStringPrintTestAction::createAction() const
{
    return std::make_shared<StandardStringPrintTestAction>();
}
