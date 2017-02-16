//
// Created by megaxela on 06.02.17.
//

#include "BarcodePrintingTest.h"
#include <Testing/TestLogger.h>
#include <random>
#include <include/Tools/Time.h>
#include <thread>
#include <chrono>

BarcodePrintingTest::BarcodePrintingTest(TestEnvironment *environment) :
    AbstractTest(environment, "Тест проверяющий печать штрих-кодов.",
                 "Печатаются штрих-коды со случайными номерами штрих-кодов.",
                 {{"Password", (uint32_t) 30},
                  {"Number Of Barcodes", (uint32_t) 150},
                  {"Seed", (uint64_t) 0}})
{

}

bool BarcodePrintingTest::execute()
{
    auto password = getValueUInt32("Password");
    auto seed = getValueUInt64("Seed");
    auto numberOfBarcodes = getValueUInt32("Number Of Barcodes");

    // Включаем печать, иначе какой смысл
    auto before = enviroment()->tools()->enablePrinting(
            password,
            TestingTools::Printing::Enabled
    );

    if (before == TestingTools::Printing::Unknown)
    {
        enviroment()->logger()->log("Не удалось включить печать.");
        return false;
    }

    // Creating random number generator
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(000000000000, 999999999999);

    if (seed == 0)
    {
        seed = (uint32_t) Time::getMilliseconds();
    }

    enviroment()->logger()->log("Используется seed: " + std::to_string(seed));

    generator.seed(seed);

    for (uint32_t i = 0; i < numberOfBarcodes; ++i)
    {
        if (enviroment()->tools()->testingStoped())
        {
            enviroment()->logger()->log("Восстанавливаем состояние печати.");
            if (before != TestingTools::Printing::Enabled)
            {
                before = enviroment()->tools()->enablePrinting(
                        password,
                        before
                );

                if (before == TestingTools::Printing::Unknown)
                {
                    enviroment()->logger()->log("Не удалось вернуть состояние печати. Останавливает тест.");
                    return false;
                }
            }
            enviroment()->logger()->log("Тестирование остановлено");

            return true;
        }

        auto value = distribution(generator);
        if (!enviroment()->driver()->standardStringPrint(
                password,
                1,
                "Value: " + std::to_string(value) + " on barcode #" + std::to_string(i)
        ))
        {
            enviroment()->logger()->log(
                    "Не удалось напечатать значение. Ошибка: #" +
                    std::to_string((int) enviroment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString(
                            (int) enviroment()->driver()->getLastError()
                    )
            );

            return false;
        }

        enviroment()->logger()->log("Печатается штрих-код со значением " + std::to_string(value) + " на штрихкоде №" + std::to_string(i) + "...");
        if (!enviroment()->driver()->printBarcode(
                password,
                value
        ))
        {
            enviroment()->logger()->log(
                    "Не удалось напечатать штрих-код со значением " +
                    std::to_string(value) +
                    " Ошибка #" +
                    std::to_string((int) enviroment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString(
                            (int) enviroment()->driver()->getLastError()
                    )
            );

            return false;
        }

        Time::sleep<std::chrono::milliseconds>(150);
    }

    if (before != TestingTools::Printing::Enabled)
    {
        before = enviroment()->tools()->enablePrinting(
                password,
                before
        );

        if (before == TestingTools::Printing::Unknown)
        {
            enviroment()->logger()->log("Не удалось вернуть состояние печати. Останавливает тест.");
            return false;
        }
    }

    return true;
}
