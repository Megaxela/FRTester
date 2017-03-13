//
// Created by megaxela on 22.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/SendRawTestAction.h"

REGISTER_ACTION(SendRawTestAction)

SendRawTestAction::SendRawTestAction() :
    AbstractTestAction("Отправка байт",
                       "Действие для отправки чистых байт.",
                       {{"Bytes", ByteArray()}},
                       {CATEGORY_ADDITIONAL})
{

}

SendRawTestAction::~SendRawTestAction()
{

}

bool SendRawTestAction::execute()
{
    environment()->driver()->sendRaw(getValue("Bytes").toByteArray());
    return true;
}

TestActionPtr SendRawTestAction::createAction() const
{
    return std::make_shared<SendRawTestAction>();
}
