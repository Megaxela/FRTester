//
// Created by megaxela on 22.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/ExitIfTestAction.h"

REGISTER_ACTION(ExitIfTestAction)

ExitIfTestAction::ExitIfTestAction() :
    AbstractTestAction("Выйти, если...",
                       "Тест будет прерван, если выполнится условие.",
                       {{"Password", (uint32_t) 30}},
                       {CATEGORY_ACTIONS})
{
    //
}

ExitIfTestAction::~ExitIfTestAction()
{

}

bool ExitIfTestAction::execute()
{
    environment()->logger()->log("Еще не реализовано. Сорян.");
    return true;
}

TestActionPtr ExitIfTestAction::createAction() const
{
    return std::make_shared<ExitIfTestAction>();
}
