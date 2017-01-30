//
// Created by megaxela on 09.01.17.
//

#include "OperationTest.h"
#include "Testing/TestLogger.h"
#include <cmath>
#include <include/Testing/TestCore.h>
#include <include/Testing/FROperations.h>


OperationTest::OperationTest(TestEnvironment *environment)
        : AbstractTest(
            environment,
            "Тест операций",
            "Тест всех операций",
            {{"Password", (uint32_t) 30}}
        )

{

}

bool OperationTest::execute()
{
    auto password = getValueUInt32("Password");
    enviroment()->driver()->openShift(password);

    for (uint32_t operatingIndex = 0;
         operatingIndex < 4;
         ++operatingIndex)
    {
        uint32_t sum = 0;

        std::string actionName;

        switch (operatingIndex)
        {
        case 0: // Продажа
            actionName = "продажа";
            break;

        case 1: // Покупка
            actionName = "покупка";
            break;

        case 2: // Возврат продажи
            actionName = "возврат продажи";
            break;

        case 3: // Возврат покупки
            actionName = "возврат покупки";
            break;
        default:
            actionName = "ошибка";
            break;
        }

        enviroment()->logger()->log("Тестирование \"" +
                                    actionName +
                                    "\" по отделам.");

        for (uint8_t departmentIndex = 1;
             departmentIndex <= 16;
             ++departmentIndex)
        {
            uint32_t count = random(1000, 5000);
            uint32_t money = random(1000, 500000);

            double multi = FROperations::smartRound(
                    money * (count * 0.001)
            );

            enviroment()->logger()->log(
                    "Значения: (Количество * Сумма) " +
                    std::to_string(count * 0.001) +
                    " * " +
                    std::to_string(money * 0.01) +
                    " = " +
                    std::to_string(multi * 0.01)
            );

            int tries = 10;
            do
            {
                switch (operatingIndex)
                {
                case 0: // Продажа
                    enviroment()->driver()->sell(
                            password,
                            count, // Количество
                            money, // Цена
                            departmentIndex,    // Отдел
                            1,    // Первый налог
                            1,    // Второй налог
                            1,    // Третий налог
                            1,    // Четвертый налог
                            ""    // Название
                    );
                    break;

                case 1: // Покупка
                    enviroment()->driver()->buy(
                            password,
                            count, // Количество
                            money, // Цена
                            departmentIndex,    // Отдел
                            1,    // Первый налог
                            1,    // Второй налог
                            1,    // Третий налог
                            1,    // Четвертый налог
                            ""    // Название
                    );
                    break;


                case 2: // Возврат покупки
                    enviroment()->driver()->returnBuy(
                            password,
                            count, // Количество
                            money, // Цена
                            departmentIndex,    // Отдел
                            1,    // Первый налог
                            1,    // Второй налог
                            1,    // Третий налог
                            1,    // Четвертый налог
                            ""    // Название
                    );
                    break;

                case 3: // Возврат продажи
                    enviroment()->driver()->returnBuy(
                            password,
                            count, // Количество
                            money, // Цена
                            departmentIndex,    // Отдел
                            1,    // Первый налог
                            1,    // Второй налог
                            1,    // Третий налог
                            1,    // Четвертый налог
                            ""    // Название
                    );
                    break;
                default:break;
                }
            }
            while (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError && (--tries) > 0);

            sum += multi;

            if (tries == 0)
            {
                enviroment()->logger()->log(
                        "Не удалось произвести операцию \"" +
                        actionName +
                        "\". Последняя ошибка: #" +
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

        enviroment()->logger()->log("Закрытие чека с суммой: " + std::to_string(sum * 0.01));

        FRDriver::CheckResult closeResult;
        {
            int tries = 10;
            do
            {
                closeResult = enviroment()->driver()->closeCheck(
                        password,
                        sum, // Оплата типом оплаты налом
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        ""
                );
            }
            while(enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError && (--tries) > 0);

            if (tries == 0)
            {
                enviroment()->logger()->log(
                        "Не удалось закрыть чек после 10 попыток. Последняя ошибка: " +
                        FRDriver::Converters::errorToString(
                                (int) enviroment()->driver()->getLastError()
                        )
                );

                return false;
            }
        }

        CHECK_TRIGGERS;

        enviroment()->logger()->log(
                "Сдача: " +
                std::to_string(closeResult.change) +
                ", должна быть 0."
        );

        if (closeResult.change != 0)
        {
            enviroment()->logger()->log("Сдача != 0. Где-то что-то было не правильно посчитано.");
            return false;
        }
    }

    return true;
}

uint32_t OperationTest::random(uint32_t begin, uint32_t end)
{
    std::uniform_int_distribution<uint32_t> distribution(begin, end);
    return distribution(m_generator);
}


