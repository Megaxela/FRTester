//
// Created by megaxela on 11.01.17.
//

#include <include/Testing/TestCore.h>
#include <include/Testing/FROperations.h>
#include <thread>
#include <chrono>
#include "CheckLoaderTest.h"
#include "Testing/TestLogger.h"

CheckLoaderTest::CheckLoaderTest(TestEnvironment *environment) :
    AbstractTest(environment,
                 "Нагрузочный тест с операциями",
                 "Тест бьющий чеки.",
                 {{"Password", (uint32_t) 30},
                  {"Number Of Checks", (uint32_t) 50},
                  {"Number Of Operations", (uint32_t) 100},
                  {"Good Price", (uint64_t) 5000},
                  {"Good Count", (uint64_t) 1000},
                  {"Enable Print", true}})
{

}

bool CheckLoaderTest::execute()
{
    auto password = getValueUInt32("Password");
    auto numberOfChecks = getValueUInt32("Number Of Checks");
    auto numberOfOperations = getValueUInt32("Number Of Operations");
    auto goodPrice = getValueUInt64("Good Price");
    auto goodCount = getValueUInt64("Good Count");
    auto printEnabled = getValueBoolean("Enable Print");

    auto printValue = printEnabled ? TestingTools::Printing::Enabled : TestingTools::Printing::Disabled;

    auto currentPrintValue = enviroment()->tools()->enablePrinting(
            password,
            printValue
    );

    if (currentPrintValue == TestingTools::Printing::Unknown)
    {
        enviroment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
        return false;
    }

    bool needToRestore = printValue != currentPrintValue;

    enviroment()->driver()->openShift(password);

    enviroment()->logger()->log("Начинаем пробивать чеки.");

    for (uint32_t checkIndex = 0;
         checkIndex < numberOfChecks;
         ++checkIndex)
    {
        for (uint32_t operationIndex = 0;
             operationIndex < numberOfOperations;
             ++operationIndex)
        {
            // Попытка произвести действие
            {
                int tries = 25;
                int realTries = 0;
                do
                {
                    ++realTries;
                    bool result = enviroment()->driver()->sell(
                            password,
                            goodCount,
                            goodPrice,
                            1,
                            0,
                            0,
                            0,
                            0,
                            "Good #" +
                            std::to_string(operationIndex + 1) +
                            "/" +
                            std::to_string(numberOfOperations) +
                            " (" +
                            std::to_string(checkIndex) +
                            '/' +
                            std::to_string(numberOfChecks) +
                            ')'
                    );

                    if (realTries == 500)
                    {
                        enviroment()->logger()->log(
                                "Печать предыдущей команды осуществляется слишком долго."
                                " (более 500 попыток) Прерываем тест."
                        );

                        return false;
                    }

                    if ((int) enviroment()->driver()->getLastError() != 0)
                    {
                        if ((int) enviroment()->driver()->getLastError() != 80)
                        {
                            --tries;
                        }
                        else
                        {
                            std::this_thread::sleep_for(
                                    std::chrono::milliseconds(200)
                            );
                        }
                    }

                }
                while (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError &&
                       tries > 0);

                if (tries == 0)
                {
                    enviroment()->logger()->log(
                            "Не удалось произвести продажу. Последняя ошибка: #" +
                            std::to_string((int) enviroment()->driver()->getLastError()) +
                            ' ' +
                            FRDriver::Converters::errorToString(
                                    (int) enviroment()->driver()->getLastError()
                            )
                    );

                    return false;
                }

                CHECK_TRIGGERS;
            }

            if (numberOfOperations / 5 > 0)
            {
                if ((operationIndex + 1) % (numberOfOperations / 5) == 0)
                {
                    enviroment()->logger()->log(
                            "Пробито " +
                            std::to_string(operationIndex + 1) +
                            '/' +
                            std::to_string(numberOfOperations)
                    );
                }
            }
        }

        enviroment()->logger()->log(
                "Пробит весь чек. Закрываем его."
        );

        {
            int tries = 10;

            do
            {
                enviroment()->driver()->closeCheck(
                        password,
                        FROperations::smartRound(
                                goodPrice * (goodCount * 0.001)
                        ) * numberOfOperations,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        "Check #" +
                        std::to_string(checkIndex) +
                        '/' +
                        std::to_string(numberOfChecks)
                );

                if ((int) enviroment()->driver()->getLastError() != 0 &&
                    (int) enviroment()->driver()->getLastError() != 80)
                {
                    --tries;
                }
            }
            while (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError &&
                   tries > 0);

            if (tries == 0)
            {
                enviroment()->logger()->log(
                        "Ошибка на чеке " +
                        std::to_string(checkIndex) +
                        '/' +
                        std::to_string(numberOfChecks)
                );

                enviroment()->logger()->log(
                        "Не удалось закрыть чек в течении 10 попыток. Последняя ошибка: #" +
                        std::to_string((int) enviroment()->driver()->getLastError()) +
                        ' ' +
                        FRDriver::Converters::errorToString(
                                (int) enviroment()->driver()->getLastError()
                        )
                );

                return false;
            }

            CHECK_TRIGGERS;
        }

        enviroment()->logger()->log(
                "Пробит чек " +
                std::to_string(checkIndex + 1) +
                '/' +
                std::to_string(numberOfChecks)
        );
    }

    if (needToRestore)
    {
        currentPrintValue = enviroment()->tools()->enablePrinting(password, currentPrintValue);
        if (currentPrintValue == TestingTools::Printing::Unknown)
        {
            enviroment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
            return false;
        }
    }

    return true;
}
