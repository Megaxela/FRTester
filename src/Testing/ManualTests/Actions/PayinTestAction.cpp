//
// Created by megaxela on 27.02.17.
//


#include <include/Testing/ManualTests/Actions/PayinTestAction.h>
#include <include/Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(PayinTestAction);

PayinTestAction::PayinTestAction() :
    AbstractTestAction("Внесение",
                       "Внесение денежной суммы",
                       {{"Пароль", (uint32_t) 30},
                        {"Сумма", (uint64_t) 0}},
                       {CATEGORY_ACTIONS})
{

}

PayinTestAction::~PayinTestAction()
{

}

bool PayinTestAction::execute()
{
    environment()->driver()->payin(
            getValue("Пароль").toUInt32(),
            getValue("Сумма").toUInt64()
    )
    return true;
}

TestActionPtr PayinTestAction::createAction() const
{
    return std::make_shared<PayinTestAction>();
}
