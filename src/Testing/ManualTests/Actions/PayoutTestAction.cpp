//
// Created by megaxela on 27.02.17.
//

#include <Testing/ManualTests/AbstractTestAction.h>
#include <Testing/ManualTests/Actions/PayoutTestAction.h>
#include <Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(PayoutTestAction)

PayoutTestAction::PayoutTestAction() :
    AbstractTestAction("Выплата",
                       "",
                       {{"Password", (uint32_t) 30},
                        {"Sum", (uint64_t) 0}},
                       {CATEGORY_ACTIONS})
{

}

PayoutTestAction::~PayoutTestAction()
{

}

bool PayoutTestAction::execute()
{
    environment()->driver()->payout(
            getValue("Password").toUInt32(),
            getValue("Sum").toUInt64()
    );

    return true;
}

TestActionPtr PayoutTestAction::createAction() const
{
    return std::make_shared<PayoutTestAction>();
}
