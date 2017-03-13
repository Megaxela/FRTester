//
// Created by megaxela on 27.02.17.
//

#include <include/Testing/ManualTests/TestActionFabric.h>
#include "include/Testing/ManualTests/Actions/ShortStateRequest.h"

REGISTER_ACTION(ShortStateRequest)

ShortStateRequest::ShortStateRequest() :
    AbstractTestAction("Короткий запрос состояния",
                        "",
                       {{"Password", (uint32_t) 30},
                        {"Print?", (bool) false}},
                       {CATEGORY_ACTIONS})
{

}

ShortStateRequest::~ShortStateRequest()
{

}

bool ShortStateRequest::execute()
{
    auto result = environment()->driver()->shortStateRequest(
            getValue("Password").toUInt32()
    );

    if (getValue("Print?").toBoolean())
    {
        if (environment()->driver()->getLastError() == FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Текущее состояние: " +
                    FRDriver::Converters::posModeToString(
                            result.posMode
                    )
            );
            environment()->logger()->log(
                    "Подсостояние: " +
                    FRDriver::Converters::posSubModeToString(
                            result.posSubMode
                    )
            );
        }
        else
        {
            environment()->logger()->log(
                    "Во время запроса была получена ошибка. Ошибка: #" +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError())
            );
        }

    }

    return true;
}

TestActionPtr ShortStateRequest::createAction() const
{
    return std::make_shared<ShortStateRequest>();
}
