//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include <Tools/Codecs.h>
#include "Testing/ManualTests/Actions/StandardStringPrintTestAction.h"

REGISTER_ACTION(StandardStringPrintTestAction)

StandardStringPrintTestAction::StandardStringPrintTestAction() :
    AbstractTestAction("Печать строки",
                        "",
                       {{"Password", (uint32_t) 30},
                        {"String", std::string("")}},
                       {CATEGORY_ACTIONS})
{

}

StandardStringPrintTestAction::~StandardStringPrintTestAction()
{

}

bool StandardStringPrintTestAction::execute()
{
    environment()->driver()->standardStringPrint(
            getValue("Password").toUInt32(),
            1,
            Codecs::instance().convert(
                    "utf-8",
                    "cp1251",
                    getValue("String").toString().c_str()
            ).toStdString()
    );

    return true;
}

TestActionPtr StandardStringPrintTestAction::createAction() const
{
    return std::make_shared<StandardStringPrintTestAction>();
}
