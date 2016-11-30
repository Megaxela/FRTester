//
// Created by megaxela on 22.11.16.
//

#include <Tools/Logger.h>
#include "CycleTest.h"


CycleTest::CycleTest(TestEnvironment *enviroment) :
        AbstractTest(
                enviroment,
                "Тест на зацикливание печати при закрытии чека.",
                "При оплате чека с 1 товаром стоимостью 16 суммами "
                "4/4/4/4 то ФР уходил в цикл."
        )
{

}

bool CycleTest::execute()
{
    if (!enviroment()->driver()->openShift(30))
    {
        Error("Не удалось открыть смену.");
        return false;
    }

    if (!enviroment()->driver()->sell(
            30, // Пароль
            100, // 1 шт
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

    auto result = enviroment()->driver()->closeCheck(
            30, // Пароль
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

    if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        Error("Не удалось закрыть чек. Предположительно ФР сейчас умрет.");
        return false;
    }

    if (result.change != 0)
    {
        Error("Сдача не равна 0. Ошибочка.");
        return false;
    }

    return true;
}
