//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/RebootTestAction.h"

REGISTER_ACTION(RebootTestAction);

RebootTestAction::RebootTestAction() :
    AbstractTestAction("Перезагрузка ФР",
                        "",
                       {},
                       {CATEGORY_ACTIONS})
{

}

RebootTestAction::~RebootTestAction()
{

}

bool RebootTestAction::execute()
{
    environment()->driver()->reboot();

    return true;
}

TestActionPtr RebootTestAction::createAction() const
{
    return std::make_shared<RebootTestAction>();
}
