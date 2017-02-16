//
// Created by megaxela on 07.02.17.
//

#include "CheckFontTest.h"
#include <Testing/TestLogger.h>

CheckFontTest::CheckFontTest(TestEnvironment* environment) :
    AbstractTest(environment,
                 "Тест, проверяющий все шрифты в чеке.",
                 "Тест перебирает все шрифты в 8 таблице.",
                 {{"Password", (uint32_t) 30}})
{

}

bool CheckFontTest::execute()
{
    auto password = getValueUInt32("Password");
    // Открываем смену
    if (!enviroment()->driver()->openShift(password))
    {
        enviroment()->logger()->log("Не удалось открыть смену. Ошибка #" +
                                    std::to_string((int) enviroment()->driver()->getLastError()) +
                                    ' ' +
                                    FRDriver::Converters::errorToString((int) enviroment()->driver()->getLastError()));
        return false;
    }

    if (!enviroment()->tools()->waitForPrintingFinished(password, 5000))
    {
        enviroment()->logger()->log(
                "Не удалось дождаться окончания печати документа открытия смены."
        );

        return false;
    }

    for (uint8_t field = 1; field <= 22; ++field)
    {
        // Сохраняем прошлое значение.
        uint8_t lastValue = readFont(password, field);

        for (uint8_t fontValue = 1; fontValue <= 8; ++fontValue)
        {
            // Записываем новое значение
            if (!writeFont(password, field, fontValue))
            {
                enviroment()->logger()->log("Не удалось записать шрифт. Останавливаем тестирование.");
                return false;
            }

            for (uint8_t sellIndex = 0; sellIndex < 3; ++sellIndex)
            {
                enviroment()->logger()->log(
                        "Совершаем продажу с №" +
                        std::to_string(sellIndex + 1) +
                        " со шрифтом " +
                        std::to_string(fontValue) +
                        " в поле " +
                        std::to_string(field)
                );

                if (!enviroment()->driver()->sell(
                        password,
                        1000,
                        1000,
                        1,
                        0,
                        0,
                        0,
                        0,
                        "SOME TOOL"
                ))
                {
                    enviroment()->logger()->log("Не удалось совершить продажу. Ошибка #" +
                                                std::to_string((int) enviroment()->driver()->getLastError()) +
                                                ' ' +
                                                FRDriver::Converters::errorToString((int) enviroment()->driver()->getLastError()));
                    return false;
                }

                // Закрытие чека
                enviroment()->driver()->closeCheck(
                        password,
                        1000,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        "#" + std::to_string(sellIndex + 1) +
                        " Font:" + std::to_string(fontValue) +
                        " Field:" + std::to_string(field)
                );

                if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
                {
                    enviroment()->logger()->log("Не удалось закрыть чек. Ошибка #" +
                                                std::to_string((int) enviroment()->driver()->getLastError()) +
                                                ' ' +
                                                FRDriver::Converters::errorToString((int) enviroment()->driver()->getLastError()));

                    return false;
                }

                if (!enviroment()->tools()->waitForPrintingFinished(password, 5000))
                {
                    enviroment()->logger()->log("Мы не дождались окончания печати.");
                    return false;
                }
            }
        }

        // Возвращаем старое значение
        if (!writeFont(password, field, lastValue))
        {
            enviroment()->logger()->log("Не удалось восстановить шрифт. Останавливаем тестирование.");
            return false;
        }
    }

    return true;
}

uint8_t CheckFontTest::readFont(uint32_t pwd, uint8_t field)
{
    auto value = enviroment()->driver()->readTableBin(
            pwd,
            8,
            1,
            field
    );

    if (enviroment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        enviroment()->logger()->log("Не удалось получить " + std::to_string(field) + " значение таблицы №8. Ошибка: #" +
                                    std::to_string((int) enviroment()->driver()->getLastError()) +
                                    ' ' +
                                    FRDriver::Converters::errorToString((int) enviroment()->driver()->getLastError()));

        return 0;
    }

    return (uint8_t) value;
}

bool CheckFontTest::writeFont(uint32_t pwd, uint8_t field, uint8_t value)
{
    if (!enviroment()->driver()->writeTable(
            pwd,
            8,
            1,
            field,
            value,
            1
    ))
    {
        enviroment()->logger()->log("Не удалось записать значение " + std::to_string(value) + " в поле " + std::to_string(field) + " в таблицы №8. Ошибка: #" +
                                    std::to_string((int) enviroment()->driver()->getLastError()) +
                                    ' ' +
                                    FRDriver::Converters::errorToString((int) enviroment()->driver()->getLastError()));

        return false;
    }

    return true;
}