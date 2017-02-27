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
    uint32_t lastSSR = 0;

    environment()->driver()->openShift(getValue("Пароль").toUInt32());

    for (uint32_t i = 0; i < result; ++i)
    {
        environment()->driver()->sell(
                getValue("Пароль").toUInt32(),
                1,
                1000,
                1,
                0,
                0,
                0,
                0,
                "TEST"
        );

        environment()->driver()->closeCheck(
                getValue("Пароль").toUInt32(),
                1000,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                "Text to print"
        );

        environment()->tools()->waitForPrintingFinished(
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
                    std::to_string(result) +
                    " (" +
                    std::to_string(i - lastSSR) +
                    " запросов в секунду)"
            );
            lastSSR = i;
        }
    }

    return true;
}
