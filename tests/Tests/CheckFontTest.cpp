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
    if (!environment()->driver()->openShift(password))
    {
        environment()->logger()->log("Не удалось открыть смену. Ошибка #" +
                                    std::to_string((int) environment()->driver()->getLastError()) +
                                    ' ' +
                                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError()));
        return false;
    }

    if (!environment()->tools()->waitForPrintingFinished(password, 5000))
    {
        environment()->logger()->log(
                "Не удалось дождаться окончания печати документа открытия смены."
        );

        return false;
    }

    for (uint8_t field = 1; field <= 22; ++field)
    {

        CHECK_IS_TEST_RUNNING;
        // Сохраняем прошлое значение.
        uint8_t lastValue = readFont(password, field);

        for (uint8_t fontValue = 1; fontValue <= 8; ++fontValue)
        {
            CHECK_IS_TEST_RUNNING;

            // Записываем новое значение
            if (!writeFont(password, field, fontValue))
            {
                environment()->logger()->log("Не удалось записать шрифт. Останавливаем тестирование.");
                return false;
            }

            environment()->logger()->log(
                    "Совершаем продажу"
                    " со шрифтом " +
                    std::to_string(fontValue) +
                    " в поле " +
                    std::to_string(field)
            );

            if (!environment()->driver()->sell(
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
                environment()->logger()->log("Не удалось совершить продажу. Ошибка #" +
                                            std::to_string((int) environment()->driver()->getLastError()) +
                                            ' ' +
                                            FRDriver::Converters::errorToString((int) environment()->driver()->getLastError()));
                return false;
            }

            // Закрытие чека
            environment()->driver()->closeCheck(
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
                    " Font:" + std::to_string(fontValue) +
                    " Field:" + std::to_string(field)
            );

            if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
            {
                environment()->logger()->log("Не удалось закрыть чек. Ошибка #" +
                                            std::to_string((int) environment()->driver()->getLastError()) +
                                            ' ' +
                                            FRDriver::Converters::errorToString((int) environment()->driver()->getLastError()));

                return false;
            }

            if (!environment()->tools()->waitForPrintingFinished(password, 5000))
            {
                environment()->logger()->log("Мы не дождались окончания печати.");
                return false;
            }
        }

        // Возвращаем старое значение
        if (!writeFont(password, field, lastValue))
        {
            environment()->logger()->log("Не удалось восстановить шрифт. Останавливаем тестирование.");
            return false;
        }
    }

    return true;
}

uint8_t CheckFontTest::readFont(uint32_t pwd, uint8_t field)
{
    auto value = environment()->driver()->readTableBin(
            pwd,
            8,
            1,
            field
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log("Не удалось получить " + std::to_string(field) + " значение таблицы №8. Ошибка: #" +
                                    std::to_string((int) environment()->driver()->getLastError()) +
                                    ' ' +
                                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError()));

        return 0;
    }

    return (uint8_t) value;
}

bool CheckFontTest::writeFont(uint32_t pwd, uint8_t field, uint8_t value)
{
    if (!environment()->driver()->writeTable(
            pwd,
            8,
            1,
            field,
            value,
            1
    ))
    {
        environment()->logger()->log("Не удалось записать значение " + std::to_string(value) + " в поле " + std::to_string(field) + " в таблицы №8. Ошибка: #" +
                                    std::to_string((int) environment()->driver()->getLastError()) +
                                    ' ' +
                                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError()));

        return false;
    }

    return true;
}
