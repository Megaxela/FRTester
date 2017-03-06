//
// Created by megaxela on 22.02.17.
//

#include "include/Testing/ManualTests/Actions/SendRawTestAction.h"

REGISTER_ACTION(SendRawTestAction);

SendRawTestAction::SendRawTestAction() :
    AbstractTestAction("Отправка байт",
                       "Действие для отправки чистых байт.",
                       {{"Байты", ByteArray()}},
                       {CATEGORY_ADDITIONAL})
{

}

SendRawTestAction::~SendRawTestAction()
{

}

bool SendRawTestAction::execute()
{
    environment()->driver()->sendRaw(getValue("Байты").toByteArray());
    return false;
}

TestActionPtr SendRawTestAction::createAction() const
{
    return std::make_shared<SendRawTestAction>();
}
