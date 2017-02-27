//
// Created by megaxela on 17.02.17.
//

#include "include/Testing/ManualTests/Actions/BeepTestAction.h"

BeepTestAction::BeepTestAction() :
    AbstractTestAction("Гудок",
                       "Действие, выполняющее гудок.",
                       {{"Password", (uint32_t) 30}})
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
