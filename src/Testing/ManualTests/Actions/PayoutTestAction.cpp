//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/AbstractTestAction.h>
#include <include/Testing/ManualTests/Actions/PayoutTestAction.h>
#include <include/Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(PayoutTestAction);

PayoutTestAction::PayoutTestAction() :
    AbstractTestAction("Выплата",
                       "",
                       {{"Пароль", (uint32_t) 30},
                        {"Сумма", (uint64_t) 0}},
                       {CATEGORY_ACTIONS})
{

}

PayoutTestAction::~PayoutTestAction()
{

}

bool PayoutTestAction::execute()
{
    environment()->driver()->payout(
            getValue("Пароль").toUInt32(),
            getValue("Сумма").toUInt64()
    );

    return true;
}

TestActionPtr PayoutTestAction::createAction() const
{
    return std::make_shared<PayoutTestAction>();
}
