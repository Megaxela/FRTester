//
// Created by megaxela on 27.02.17.
//


#include <include/Testing/ManualTests/Actions/PayinTestAction.h>
#include <include/Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(PayinTestAction)

PayinTestAction::PayinTestAction() :
    AbstractTestAction("Внесение",
                       "Внесение денежной суммы",
                       {{"Password", (uint32_t) 30},
                        {"Sum", (uint64_t) 0}},
                       {CATEGORY_ACTIONS})
{

}

PayinTestAction::~PayinTestAction()
{

}

bool PayinTestAction::execute()
{
    environment()->driver()->payin(
            getValue("Password").toUInt32(),
            getValue("Sum").toUInt64()
    );

    return true;
}

TestActionPtr PayinTestAction::createAction() const
{
    return std::make_shared<PayinTestAction>();
}
