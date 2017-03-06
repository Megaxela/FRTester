//
// Created by megaxela on 21.02.17.
//

#include "include/Testing/ManualTests/Actions/LongStateRequestTestAction.h"
#include <Testing/TestLogger.h>
#include <include/Testing/ManualTests/TestActionFabric.h>

REGISTER_ACTION(LongStateRequestTestAction);

LongStateRequestTestAction::LongStateRequestTestAction() :
    AbstractTestAction("Длинный запрос состояния",
                       "Действие, выполняющее длинный запрос состояния",
                       {{"Password", (uint32_t) 30},
                        {"Print stats", true}},
                       {CATEGORY_ACTIONS})
{

}

LongStateRequestTestAction::~LongStateRequestTestAction()
{

}

bool LongStateRequestTestAction::execute()
{
    auto result = environment()->driver()->fullStateRequest(
            getValue("Password").toUInt32()
    );

    if (getValue("Print stats").toBoolean())
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

TestActionPtr LongStateRequestTestAction::createAction() const
{
    return std::make_shared<LongStateRequestTestAction>();
}
