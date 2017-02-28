//
// Created by megaxela on 11.01.17.
//

#include <include/Testing/TestCore.h>
#include <include/Testing/FROperations.h>
#include <thread>
#include <chrono>
#include <include/Tools/Time.h>
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
                  {"Discount", (uint16_t) 0},
                  {"Manual close", false},
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
    auto manualClose = getValueBoolean("Manual close");
    auto discount = getValue("Discount").toUInt16();

    auto printValue = printEnabled ? TestingTools::Printing::Enabled : TestingTools::Printing::Disabled;

    auto currentPrintValue = environment()->tools()->enablePrinting(
            password,
            printValue
    );

    if (currentPrintValue == TestingTools::Printing::Unknown)
    {
        environment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
        return false;
    }

    bool needToRestore = printValue != currentPrintValue;

    environment()->driver()->openShift(password);

    environment()->logger()->log("Начинаем пробивать чеки.");

    for (uint32_t checkIndex = 0;
         checkIndex < numberOfChecks;
         ++checkIndex)
    {
        if (environment()->tools()->testingStoped())
        {
            // Восстанавливаем состояние печати
            if (needToRestore)
            {
                currentPrintValue = environment()->tools()->enablePrinting(password, currentPrintValue);
                if (currentPrintValue == TestingTools::Printing::Unknown)
                {
                    environment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
                    return false;
                }
            }

            environment()->logger()->log("Тестирование остановлено");
            return true;
        }

        for (uint32_t operationIndex = 0;
             operationIndex < numberOfOperations;
             ++operationIndex)
        {
            if (environment()->tools()->testingStoped())
            {
                // Восстанавливаем состояние печати
                if (needToRestore)
                {
                    currentPrintValue = environment()->tools()->enablePrinting(password, currentPrintValue);
                    if (currentPrintValue == TestingTools::Printing::Unknown)
                    {
                        environment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
                        return false;
                    }
                }

                environment()->logger()->log("Тестирование остановлено");
                return true;
            }

            // Попытка произвести действие
            {
                int tries = 25;
                int realTries = 0;
                do
                {
                    ++realTries;
                    bool result = environment()->driver()->sell(
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
                        environment()->logger()->log(
                                "Печать предыдущей команды осуществляется слишком долго."
                                " (более 500 попыток) Прерываем тест."
                        );

                        return false;
                    }

                    if ((int) environment()->driver()->getLastError() != 0)
                    {
                        if ((int) environment()->driver()->getLastError() != 80)
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
                while (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError &&
                       tries > 0);

                if (tries == 0)
                {
                    environment()->logger()->log(
                            "Не удалось произвести продажу. Последняя ошибка: #" +
                            std::to_string((int) environment()->driver()->getLastError()) +
                            ' ' +
                            FRDriver::Converters::errorToString(
                                    (int) environment()->driver()->getLastError()
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
                    environment()->logger()->log(
                            "Пробито " +
                            std::to_string(operationIndex + 1) +
                            '/' +
                            std::to_string(numberOfOperations)
                    );
                }
            }
        }

        environment()->logger()->log(
                "Пробит весь чек. Закрываем его."
        );

        time_t checkCloseTime = 0;
        time_t printTime = 0;

        {
            int tries = 10;

            do
            {
                if (environment()->tools()->testingStoped())
                {
                    // Восстанавливаем состояние печати
                    if (needToRestore)
                    {
                        currentPrintValue = environment()->tools()->enablePrinting(password, currentPrintValue);
                        if (currentPrintValue == TestingTools::Printing::Unknown)
                        {
                            environment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
                            return false;
                        }
                    }

                    environment()->logger()->log("Тестирование остановлено");
                    return true;
                }

                if (manualClose)
                {
                    if (!environment()->tools()->messageQuestion(
                            "Закрыть чек или прервать тестирование?",
                            "Закрыть чек",
                            "Прервать тест"
                    ))
                    {
                        return false;
                    }
                }

                checkCloseTime = Time::timeFunction<std::chrono::milliseconds>(
                        [=]()
                        {
                            environment()->driver()->closeCheck(
                                    password,
                                    FROperations::smartRound(
                                            goodPrice * (goodCount * 0.001)
                                    ) * numberOfOperations,
                                    0,
                                    0,
                                    0,
                                    discount,
                                    0,
                                    0,
                                    0,
                                    0,
                                    "Check #" +
                                    std::to_string(checkIndex) +
                                    '/' +
                                    std::to_string(numberOfChecks)
                            );
                        }
                );

                if (environment()->driver()->getLastError() == FRDriver::ErrorCode::NoError)
                {
                    printTime = Time::timeFunction<std::chrono::milliseconds>(
                            [=]()
                            {
                                environment()->tools()->waitForPrintingFinished(
                                        password,
                                        5000
                                );
                            }
                    );
                }

                if ((int) environment()->driver()->getLastError() != 0 &&
                    (int) environment()->driver()->getLastError() != 80)
                {
                    --tries;
                }
            }
            while (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError &&
                   tries > 0);

            if (tries == 0)
            {
                environment()->logger()->log(
                        "Ошибка на чеке " +
                        std::to_string(checkIndex) +
                        '/' +
                        std::to_string(numberOfChecks)
                );

                environment()->logger()->log(
                        "Не удалось закрыть чек в течении 10 попыток. Последняя ошибка: #" +
                        std::to_string((int) environment()->driver()->getLastError()) +
                        ' ' +
                        FRDriver::Converters::errorToString(
                                (int) environment()->driver()->getLastError()
                        )
                );

                return false;
            }

            if (environment()->tools()->testingStoped())
            {
                // Восстанавливаем состояние печати
                if (needToRestore)
                {
                    currentPrintValue = environment()->tools()->enablePrinting(password, currentPrintValue);
                    if (currentPrintValue == TestingTools::Printing::Unknown)
                    {
                        environment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
                        return false;
                    }
                }

                environment()->logger()->log("Тестирование остановлено");
                return true;
            }
        }

        environment()->logger()->log(
                "Пробит чек " +
                std::to_string(checkIndex + 1) +
                '/' +
                std::to_string(numberOfChecks) +
                " (время закрытия " +
                std::to_string(checkCloseTime) +
                " мс, время печати " +
                std::to_string(printTime) +
                ")."
        );
    }

    if (needToRestore)
    {
        currentPrintValue = environment()->tools()->enablePrinting(password, currentPrintValue);
        if (currentPrintValue == TestingTools::Printing::Unknown)
        {
            environment()->logger()->log("Не удалось изменить состояние печати. Останавлиаем тест.");
            return false;
        }
    }

    return true;
}
