//
// Created by megaxela on 22.02.17.
//

#include "include/Testing/ManualTests/Actions/NotifyWindowTestAction.h"

NotifyWindowTestAction::NotifyWindowTestAction() :
    AbstractTestAction("Уведомление пользователя",
                       "Действие, уведомляющее пользователя о чем-то.",
                       {{"Запрос", std::string("Текст уведомления")}})
{

}

NotifyWindowTestAction::~NotifyWindowTestAction()
{

}

bool NotifyWindowTestAction::execute()
{
    environment()->tools()->messageNotify(
            getValue("Запрос").toString()
    );
    return true;
}

TestActionPtr NotifyWindowTestAction::createAction() const
{
    return TestActionPtr();
}
