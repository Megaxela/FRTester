//
// Created by megaxela on 26.01.17.
//

#include "NonfiscalRequesting.h"
#include "Testing/TestLogger.h"
#include <thread>
#include <chrono>
#include <include/Testing/StaticTestFabric.h>

REGISTER_STATIC_TEST(NonfiscalRequesting)

NonfiscalRequesting::NonfiscalRequesting() :
    NonfiscalRequesting(nullptr)
{

}

NonfiscalRequesting::NonfiscalRequesting(TestEnvironment* environment) :
    AbstractTest(environment,
                 "Особый тест для питерского Linux.",
                 "Тест, который выполняет: "
                 "Отрытие нефискального документ, "
                 "Печать 20 нумерованных строк, "
                 "Закрытие нефискального документа, "
                 "Отрезка чека, "
                 "20 коротких запросов.",
                 {{"Password", (uint32_t) 30},
                  {"Number Of Documents", (uint32_t) 150}})

{

}

bool NonfiscalRequesting::execute()
{
    auto password = getValueUInt32("Password");
    auto docs = getValueUInt32("Number Of Documents");

    for (uint32_t cycle = 0; cycle < docs; ++cycle)
    {

        CHECK_IS_TEST_RUNNING;

        environment()->logger()->log("Открываем нефискальный документ #" + std::to_string(cycle + 1));

        environment()->driver()->openNonFiscalDocument(password);
        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation)
        {
            environment()->logger()->log("    Каким-то образом прошло открытие нефискального документа.");
            return false;
        }

        environment()->logger()->log("    Печать текста.");

        for (uint32_t line = 0; line < 20; ++line)
        {
            auto result = environment()->driver()->fontStringPrint(password, 7, 1, "Line #" + std::to_string(line + 1));
            if (!result)
            {
                environment()->logger()->log(
                        "    Печать " +
                        std::to_string(line + 1) +
                        " строки не удалась. Ошибка: " +
                        FRDriver::Converters::errorToString((int) environment()->driver()->getLastError())
                );

                return false;
            }
        }

        environment()->logger()->log("Закрываем нефискальный документ #" + std::to_string(cycle + 1));
        environment()->driver()->closeNonFiscalDocument(password);
        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation)
        {
            environment()->logger()->log("Каким-то образом прошло открытие нефискального документа.");
            return false;
        }

        environment()->logger()->log("Делаем полную отрезку.");
        if (!environment()->driver()->cutCheck(password, 1))
        {
            environment()->logger()->log(
                    "Отрезка чека не удалась."
            );

            return false;
        }

        environment()->logger()->log("Еще раз пытаемся что-то напечатать.");
        environment()->driver()->fontStringPrint(password, 7, 1, "ERROR");

        environment()->logger()->log("Получаем короткий запрос состояния.");
        for (uint32_t times = 0; times < 20; ++times)
        {
            environment()->driver()->shortStateRequest(password);
            if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
            {
                environment()->logger()->log("Не удалось получить короткий запрос состояния.");
                return false;
            }
        }

        environment()->logger()->log(std::to_string(cycle) + " чек напечатан в буффер. Ждем 1 секунду.");
        std::this_thread::sleep_for(
                std::chrono::milliseconds(800)
        );

    }

    return true;
}
