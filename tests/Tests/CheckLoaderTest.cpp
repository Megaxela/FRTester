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

    auto initialPrintValue = enviroment()->driver()->readTableBin(
            password,
            17,
            1,
            7
    );

    if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        enviroment()->logger()->log("Не удалось получить текущее значение печати чека.");
        return false;
    }

    bool needToRestore = false;

    if ((initialPrintValue == 0 || initialPrintValue == 1) && !printEnabled)
    {
        enviroment()->logger()->log("Отключаем печать чеков.");
        if (!enviroment()->driver()->writeTable(
                password,
                17,
                1,
                7,
                2, // Значение
                1  // Размер значения
        ))
        {
            enviroment()->logger()->log("Не удалось отключить печать чеков. Пожалуй остановим тестирование.");
            return false;
        }



        needToRestore = true;
    }
    else if (initialPrintValue == 2 && printEnabled)
    {
        enviroment()->logger()->log("Включаем печать чеков.");
        if (enviroment()->driver()->writeTable(
                password,
                17,
                1,
                7,
                0, // Значение
                1  // Размер значения
        ))
        {
            enviroment()->logger()->log("Не удалось включить печать чеков. Пожалуй остановим тестирование.");
            return false;
        }

        needToRestore = true;
    }

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
                int tries = 10;
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
                            "Good #" + std::to_string(operationIndex + 1)
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
        enviroment()->logger()->log("Восстанавливаем значение печати чека.");
        if (!enviroment()->driver()->writeTable(
                password,
                17,
                1,
                7,
                initialPrintValue, // Значение
                1  // Размер значения
        ))
        {
            enviroment()->logger()->log("Не удалось восстановить состояние печати.");
            return false;
        }
    }

    return true;
}
