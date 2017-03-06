//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/FontStringPrintTestAction.h"

REGISTER_ACTION(FontStringPrintTestAction);

FontStringPrintTestAction::FontStringPrintTestAction() :
    AbstractTestAction("Тест печати шрифтом",
                       "",
                       {{"Пароль", (uint32_t) 30},
                        {"Текст", ""},
                        {"Шрифт", (uint8_t) 1}},
                       {CATEGORY_ACTIONS})
{

}

FontStringPrintTestAction::~FontStringPrintTestAction()
{

}

bool FontStringPrintTestAction::execute()
{
    environment()->driver()->fontStringPrint(
            getValue("Пароль").toUInt32(),
            1,
            getValue("Шрифт").toUInt8(),
            getValue("Текст").toString()
    );

    return true;
}

TestActionPtr FontStringPrintTestAction::createAction() const
{
    return std::make_shared<FontStringPrintTestAction>();
}
