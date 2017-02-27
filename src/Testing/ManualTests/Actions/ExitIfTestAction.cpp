//
// Created by megaxela on 22.02.17.
//

#include "include/Testing/ManualTests/Actions/ExitIfTestAction.h"

ExitIfTestAction::ExitIfTestAction() :
    AbstractTestAction("Выйти, если...",
                       "Тест будет прерван, если выполнится условие.",
                       {{"Пароль", (uint32_t) 30}})
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
