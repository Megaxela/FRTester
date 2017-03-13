//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/PrintBarcodeTestAction.h"

REGISTER_ACTION(PrintBarcodeTestAction)

PrintBarcodeTestAction::PrintBarcodeTestAction() :
    AbstractTestAction("Печать штрих-кода",
                       "",
                       {{"Password", (uint32_t) 30},
                        {"Value", (uint64_t) 0}},
                       {CATEGORY_ACTIONS})
{

}

PrintBarcodeTestAction::~PrintBarcodeTestAction()
{

}

bool PrintBarcodeTestAction::execute()
{
    environment()->driver()->printBarcode(
            getValue("Password").toUInt32(),
            getValue("Value").toUInt64()
    );

    return true;
}

TestActionPtr PrintBarcodeTestAction::createAction() const
{
    return std::make_shared<PrintBarcodeTestAction>();
}
