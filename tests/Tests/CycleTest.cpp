//
// Created by megaxela on 22.11.16.
//

#include <Tools/Logger.h>
#include <include/Testing/StaticTestFabric.h>
#include "CycleTest.h"

REGISTER_STATIC_TEST(CycleTest)

CycleTest::CycleTest() :
    CycleTest(nullptr)
{

}

CycleTest::CycleTest(TestEnvironment *enviroment) :
        AbstractTest(
                enviroment,
                "Тест на зацикливание печати при закрытии чека.",
                "При оплате чека с 1 товаром стоимостью 16 суммами "
                "4/4/4/4 то ФР уходил в цикл.",
                {{"Password", (uint32_t) 30}}
        )
{

}

bool CycleTest::execute()
{
    auto pwd = getValueUInt32("Password");

    if (!environment()->driver()->openShift(30))
    {
        Error("Не удалось открыть смену.");
        return false;
    }

    // Ожидание окончания печати
    environment()->tools()->waitForPrintingFinished(pwd, 100000);

    if (!environment()->driver()->sell(
            pwd, // Пароль
            1000, // 1 шт
            16 * 100, // 16р
            0, // 0 (1) отдел
            0, // Налоги
            0, // Налоги
            0, // Налоги
            0, // Налоги
            "SHUTDOWN"
    ))
    {
        Error("Не удалось совершить такую продажу.");
        return false;
    }

    auto result = environment()->driver()->closeCheck(
            pwd, // Пароль
            4 * 100, // Кэш
            4 * 100, // Картой
            4 * 100, // Картой
            4 * 100, // Картой
            0, // Скидка
            0, // Налоги
            0, // Налоги
            0, // Налоги
            0, // Налоги
            "EASY"
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        Error("Не удалось закрыть чек. Предположительно ФР сейчас умрет.");
        return false;
    }

    if (result.change != 0)
    {
        Error("Сдача не равна 0. Ошибочка.");
        return false;
    }

    environment()->tools()->waitForPrintingFinished(pwd, 10000);

    return true;
}
