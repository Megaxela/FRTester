//
// Created by megaxela on 27.02.17.
//


#include <Testing/ManualTests/Actions/PingTestAction.h>
#include <Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(PingTestAction)

PingTestAction::PingTestAction() :
    AbstractTestAction("Пинг",
                       "",
                       {{"URL", std::string("")}},
                       {CATEGORY_ACTIONS})
{

}

PingTestAction::~PingTestAction()
{

}

bool PingTestAction::execute()
{
    environment()->driver()->ping(
            getValue("URL").toString()
    );

    return true;
}

std::shared_ptr <AbstractTestAction> PingTestAction::createAction() const
{
    return std::shared_ptr<PingTestAction>();
}
