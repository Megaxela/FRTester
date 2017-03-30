//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/TestActionFabric.h>
#include "Testing/ManualTests/Actions/FontStringPrintTestAction.h"

REGISTER_ACTION(FontStringPrintTestAction)

FontStringPrintTestAction::FontStringPrintTestAction() :
    AbstractTestAction("Тест печати шрифтом",
                       "",
                       {{"Password", (uint32_t) 30},
                        {"Text", std::string("")},
                        {"Font", (uint8_t) 1}},
                       {CATEGORY_ACTIONS})
{

}

FontStringPrintTestAction::~FontStringPrintTestAction()
{

}

bool FontStringPrintTestAction::execute()
{
    environment()->driver()->fontStringPrint(
            getValue("Password").toUInt32(),
            1,
            getValue("Font").toUInt8(),
            getValue("Text").toString()
    );

    return true;
}

TestActionPtr FontStringPrintTestAction::createAction() const
{
    return std::make_shared<FontStringPrintTestAction>();
}
