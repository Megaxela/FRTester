//
// Created by megaxela on 17.02.17.
//

#include <include/Testing/ManualTests/FabricRegistrator.h>
#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/BeepTestAction.h"

REGISTER_ACTION(BeepTestAction)

BeepTestAction::BeepTestAction() :
    AbstractTestAction("Гудок",
                       "Действие, выполняющее гудок.",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{

}

BeepTestAction::~BeepTestAction()
{

}

bool BeepTestAction::execute()
{
    environment()->driver()->beep(getValue("Password").toUInt32());
    return true;
}

TestActionPtr BeepTestAction::createAction() const
{
    return std::make_shared<BeepTestAction>();
}
