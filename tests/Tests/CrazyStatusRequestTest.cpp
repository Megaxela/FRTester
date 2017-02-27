//
// Created by megaxela on 27.02.17.
//

#include <include/Tools/Time.h>
#include "CrazyStatusRequestTest.h"

CrazyStatusRequestTest::CrazyStatusRequestTest(TestEnvironment *environment) :
    AbstractTest(environment,
                 "Тест с сумашедшим запросом состояния.",
                 "",
                 {{"Пароль", (uint32_t) 30},
                  {"Количество", (uint32_t) 100}})
{

}

bool CrazyStatusRequestTest::execute()
{
    auto result = getValue("Количество").toUInt32();

    auto time = Time::getMilliseconds();
    for (uint32_t i = 0; i < result; ++i)
    {
        auto response = environment()->driver()->fullStateRequest(
                getValue("Пароль").toUInt32()
        );

        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log("Была получена ошибка.");
            return false;
        }

        if (Time::getMilliseconds() - time > 1000)
        {
            time += 1000;
            environment()->logger()->log(
                    "Запрошено " +
                    std::to_string(i + 1) +
                    " из " +
                    std::to_string(result)
            );
        }
    }

    return true;
}
