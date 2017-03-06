//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/PrintDocumentEndTestAction.h"

REGISTER_ACTION(PrintDocumentEndTestAction);

PrintDocumentEndTestAction::PrintDocumentEndTestAction() :
    AbstractTestAction("Печать конца документа",
                       "",
                       {{"Пароль", (uint32_t) 30},
                        {"Печатать рекламный текст", (bool) false}},
                       {CATEGORY_ACTIONS})
{

}

PrintDocumentEndTestAction::~PrintDocumentEndTestAction()
{

}

bool PrintDocumentEndTestAction::execute()
{
    environment()->driver()->printDocumentEnd(
        getValue("Пароль").toUInt32(),
        (uint8_t) getValue("Печатать рекламный текст").toBoolean()
    );
    return true;
}

TestActionPtr PrintDocumentEndTestAction::createAction() const
{
    return std::make_shared<PrintDocumentEndTestAction>();
}
