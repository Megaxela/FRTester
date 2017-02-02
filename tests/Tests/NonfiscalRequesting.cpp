//
// Created by megaxela on 26.01.17.
//

#include "NonfiscalRequesting.h"
#include "Testing/TestLogger.h"
#include <thread>
#include <chrono>

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
        enviroment()->logger()->log("Открываем нефискальный документ #" + std::to_string(cycle + 1));

        enviroment()->driver()->openNonFiscalDocument(password);
        if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation)
        {
            enviroment()->logger()->log("    Каким-то образом прошло открытие нефискального документа.");
            return false;
        }

        enviroment()->logger()->log("    Печать текста.");

        for (uint32_t line = 0; line < 20; ++line)
        {
            auto result = enviroment()->driver()->fontStringPrint(password, 7, 1, "Line #" + std::to_string(line + 1));
            if (!result)
            {
                enviroment()->logger()->log(
                        "    Печать " +
                        std::to_string(line + 1) +
                        " строки не удалась. Ошибка: " +
                        FRDriver::Converters::errorToString((int) enviroment()->driver()->getLastError())
                );

                return false;
            }
        }

        enviroment()->logger()->log("Закрываем нефискальный документ #" + std::to_string(cycle + 1));
        enviroment()->driver()->closeNonFiscalDocument(password);
        if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::PointOfSalesWrongCommandIsNotSupportedForThisPointOfSalesImplementation)
        {
            enviroment()->logger()->log("Каким-то образом прошло открытие нефискального документа.");
            return false;
        }

        enviroment()->logger()->log("Делаем полную отрезку.");
        if (!enviroment()->driver()->cutCheck(password, 1))
        {
            enviroment()->logger()->log(
                    "Отрезка чека не удалась."
            );

            return false;
        }

        enviroment()->logger()->log("Еще раз пытаемся что-то напечатать.");
        enviroment()->driver()->fontStringPrint(password, 7, 1, "ERROR");

        enviroment()->logger()->log("Получаем короткий запрос состояния.");
        for (uint32_t times = 0; times < 20; ++times)
        {
            auto state = enviroment()->driver()->shortStateRequest(password);
            if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
            {
                enviroment()->logger()->log("Не удалось получить короткий запрос состояния.");
                return false;
            }
        }

        enviroment()->logger()->log(std::to_string(cycle) + " чек напечатан в буффер. Ждем 1 секунду.");
        std::this_thread::sleep_for(
                std::chrono::milliseconds(800)
        );

    }

    return true;
}
