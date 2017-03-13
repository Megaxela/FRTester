//
// Created by megaxela on 22.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/NotifyWindowTestAction.h"

REGISTER_ACTION(NotifyWindowTestAction)

NotifyWindowTestAction::NotifyWindowTestAction() :
    AbstractTestAction("Уведомление пользователя",
                       "Действие, уведомляющее пользователя о чем-то.",
                       {{"Request", std::string("Текст уведомления")}},
                       {CATEGORY_ADDITIONAL})
{

}

NotifyWindowTestAction::~NotifyWindowTestAction()
{

}

bool NotifyWindowTestAction::execute()
{
    environment()->tools()->messageNotify(
            getValue("Request").toString()
    );
    return true;
}

TestActionPtr NotifyWindowTestAction::createAction() const
{
    return std::make_shared<NotifyWindowTestAction>();
}
