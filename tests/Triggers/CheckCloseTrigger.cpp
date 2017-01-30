//
// Created by megaxela on 10.01.17.
//

#include "CheckCloseTrigger.h"
#include <algorithm>
#include <include/Tools/ByteArrayReader.h>
#include <Testing/TestEnvironment.h>
#include <Testing/TestLogger.h>
#include <include/Tools/Time.h>

CheckCloseTrigger::CheckCloseTrigger(TestEnvironment *environment) :
    AbstractTriggerTest(
            environment,
            "Триггер закрытия чека",
            "Триггер контроллирующий правильность оперирования "
            "регистрами при закрытии чека.",
            true
    ),
    m_pwd(30),
    m_tags({"close_check"}),
    m_success(false)
{

}

void CheckCloseTrigger::onPreExecute(const std::string &realTag, const ByteArray &arguments)
{
    m_success = false;
    parseArguments(arguments);

    // Получаем подытог
    m_checkResult = environment()->driver()->checkResult(m_pwd);

    // Проверяем аргументы и пытаемся предсказать результат
    uint64_t summ = m_cashSum + m_type2Sum + m_type3Sum + m_type4Sum;

    if (summ < m_checkResult)
    {
        environment()->logger()->log(
                "Сумма оплаты (" +
                std::to_string(summ) +
                ") меньше подытога (" +
                std::to_string(m_checkResult) +
                "). Оплата не должна пройти."
        );

        m_willSucceed = false;
    }
    else
    {
        m_willSucceed = true;
    }

    // Считываем регистры
    // Считываем регистр с 0 - 63
    // Поиск номера текущей смены
    for (uint8_t registerIndex = 0; registerIndex <= 63; ++registerIndex)
    {
        auto registerValue = environment()->driver()->currencyRegisterRequest(
                m_pwd,
                registerIndex
        );

        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Не удалось получить значение регистра №" +
                    std::to_string(registerIndex) +
                    " Ошибка # " +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    " " +
                    FRDriver::Converters::errorToString(
                            (int) environment()->driver()->getLastError()
                    )
            );

            return;
        }

        if (registerValue != 0)
        {
            m_currentShiftNumber = (uint32_t) (registerIndex / 4);

            m_0to63MoneyRegister = registerValue;
            m_currentAction = (uint32_t) (registerIndex % 4);

            break;
        }
    }

    // Получаем значение регистра 121 - 181
    m_121to182MoneyRegister = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (121 + (m_currentShiftNumber * 4) + m_currentAction)
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log(
                "Не удалось получить значение регистра №" +
                std::to_string((121 + (m_currentShiftNumber * 4) + m_currentAction)) +
                " Ошибка # " +
                std::to_string((int) environment()->driver()->getLastError()) +
                " " +
                FRDriver::Converters::errorToString(
                        (int) environment()->driver()->getLastError()
                )
        );

        return;
    }

    m_193to196PayMoneyRegister = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (193 + m_currentAction)
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log(
                "Не удалось получить значение регистра №" +
                std::to_string((193 + m_currentAction)) +
                " Ошибка # " +
                std::to_string((int) environment()->driver()->getLastError()) +
                " " +
                FRDriver::Converters::errorToString(
                        (int) environment()->driver()->getLastError()
                )
        );

        return;
    }

    m_197to200PayMoneyRegister = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (197 + m_currentAction)
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log(
                "Не удалось получить значение регистра №" +
                std::to_string((197 + m_currentAction)) +
                " Ошибка # " +
                std::to_string((int) environment()->driver()->getLastError()) +
                " " +
                FRDriver::Converters::errorToString(
                        (int) environment()->driver()->getLastError()
                )
        );

        return;
    }

    m_201to204PayMoneyRegister = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (201 + m_currentAction)
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log(
                "Не удалось получить значение регистра №" +
                std::to_string((201 + m_currentAction)) +
                " Ошибка # " +
                std::to_string((int) environment()->driver()->getLastError()) +
                " " +
                FRDriver::Converters::errorToString(
                        (int) environment()->driver()->getLastError()
                )
        );

        return;
    }

    m_205to208PayMoneyRegister = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (205 + m_currentAction)
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log(
                "Не удалось получить значение регистра №" +
                std::to_string((205 + m_currentAction)) +
                " Ошибка # " +
                std::to_string((int) environment()->driver()->getLastError()) +
                " " +
                FRDriver::Converters::errorToString(
                        (int) environment()->driver()->getLastError()
                )
        );

        return;
    }

    m_totalCashRegister241 = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            241
    );

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log(
                "Не удалось получить значение регистра №" +
                std::to_string(241) +
                " Ошибка # " +
                std::to_string((int) environment()->driver()->getLastError()) +
                " " +
                FRDriver::Converters::errorToString(
                        (int) environment()->driver()->getLastError()
                )
        );

        return;
    }

//    auto nonZeroSums = environment()->driver()->getNonZeroSums();
//
//    switch (m_currentAction)
//    {
//    case 0:
//        m_nonZeroSum = nonZeroSums.firstSum;
//        break;
//    case 1:
//        m_nonZeroSum = nonZeroSums.secondSum;
//        break;
//    case 2:
//        m_nonZeroSum = nonZeroSums.thirdSum;
//        break;
//    case 3:
//        m_nonZeroSum = nonZeroSums.fourthSum;
//        break;
//    }
}

void CheckCloseTrigger::onPostExecute()
{
    if (!m_willSucceed)
    {
        environment()->logger()->log(
                "Продажа не должна была пройти."
                "Сумма оплаты < Подытога."
        );
        return;
    }

    // Ожидание окончания печати
    environment()->logger()->log("Ожидание окончания печати чека.");

    uint32_t timeout = 10000; // ms
    Time::time_t start = Time::get<std::chrono::milliseconds>();

    while (true)
    {
        auto state = environment()->driver()->shortStateRequest(m_pwd);

        if (state.posSubMode == 0)
        {
            environment()->logger()->log("Печать окончена. Начинаем проверки.");
            break;
        }

        if (Time::get<std::chrono::milliseconds>() - start >= timeout)
        {
            environment()->logger()->log("Не удалось дождаться окончания печати чека.");
            return;
        }
    }

    // Проверка регистров с 0 - 63
    auto newRegisterValue = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (m_currentShiftNumber * 4 + m_currentAction)
    );

    if (newRegisterValue != 0)
    {
        environment()->logger()->log(
                "Регистр №" +
                std::to_string(m_currentShiftNumber * 4 + m_currentAction) +
                " не очистился после закрытия чека."
        );

        return;
    }

    // Проверка регистров со 121 по 182
    newRegisterValue = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (121 + (m_currentShiftNumber * 4) + m_currentAction)
    );

    if (newRegisterValue - m_121to182MoneyRegister != m_0to63MoneyRegister)
    {
        environment()->logger()->log(
                "Регистр №" +
                std::to_string((uint8_t) (121 + (m_currentShiftNumber * 4) + m_currentAction)) +
                " не изменился на сумму оплаты. Разница != сумме оплаты (" +
                std::to_string(newRegisterValue - m_121to182MoneyRegister ) +
                " != " +
                std::to_string(m_0to63MoneyRegister)
        );

        return;
    }

    // Проверка регистров 193 по 196
    newRegisterValue = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (193 + m_currentAction)
    );

    if (newRegisterValue - m_193to196PayMoneyRegister != m_cashSum)
    {
        environment()->logger()->log(
                "Регистр №" +
                std::to_string(193 + m_currentAction) +
                " не изменился на количество наличности. " +
                std::to_string(newRegisterValue - m_193to196PayMoneyRegister) +
                " != " +
                std::to_string(m_cashSum)
        );

        return;
    }

    // Проверка регистров 197 по 200
    newRegisterValue = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (197 + m_currentAction)
    );

    if (newRegisterValue - m_197to200PayMoneyRegister != m_type2Sum)
    {
        environment()->logger()->log(
                "Регистр №" +
                std::to_string(197 + m_currentAction) +
                " не изменился на количество значения второго типа оплаты. " +
                std::to_string(newRegisterValue - m_197to200PayMoneyRegister) +
                " != " +
                std::to_string(m_type2Sum)
        );

        return;
    }

    // Проверка регистров 201 по 204
    newRegisterValue = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (201 + m_currentAction)
    );

    if (newRegisterValue - m_201to204PayMoneyRegister != m_type3Sum)
    {
        environment()->logger()->log(
                "Регистр №" +
                std::to_string(201 + m_currentAction) +
                " не изменился на количество значения третьего типа оплаты. " +
                std::to_string(newRegisterValue - m_197to200PayMoneyRegister) +
                " != " +
                std::to_string(m_type3Sum)
        );

        return;
    }

    // Проверка регистров 205 по 208
    newRegisterValue = environment()->driver()->currencyRegisterRequest(
            m_pwd,
            (uint8_t) (205 + m_currentAction)
    );

    if (newRegisterValue - m_205to208PayMoneyRegister != m_type4Sum)
    {
        environment()->logger()->log(
                "Регистр №" +
                std::to_string(201 + m_currentAction) +
                " не изменился на количество значения четвертого типа оплаты. " +
                std::to_string(newRegisterValue - m_197to200PayMoneyRegister) +
                " != " +
                std::to_string(m_type4Sum)
        );

        return;
    }

    // Проверка необнуляемых сумм
//    auto newZeroSum = environment()->driver()->getNonZeroSums();
//
//    uint64_t nonZeroSumValue = 0;
//
//    switch (m_currentAction)
//    {
//    case 0:
//        nonZeroSumValue = newZeroSum.firstSum;
//        break;
//    case 1:
//        nonZeroSumValue = newZeroSum.secondSum;
//        break;
//    case 2:
//        nonZeroSumValue = newZeroSum.thirdSum;
//        break;
//    case 3:
//        nonZeroSumValue = newZeroSum.fourthSum;
//        break;
//    }
//
//    if (nonZeroSumValue - m_nonZeroSum != m_0to63MoneyRegister)
//    {
//        environment()->logger()->log(
//                "Необнуляемые суммы изменились на " + std::to_string(nonZeroSumValue - m_nonZeroSum)
//                + " вместо " + std::to_string(m_0to63MoneyRegister)
//        );
//
//        return;
//    }

    m_success = true;
}

bool CheckCloseTrigger::succeed()
{
    return m_success;
}

bool CheckCloseTrigger::containsTag(const std::string &tag)
{
    return std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end();
}

void CheckCloseTrigger::parseArguments(const ByteArray &data)
{
    ByteArrayReader reader(data);
    m_argPwd = reader.read<uint32_t>(ByteArray::ByteOrder_LittleEndian);
    m_cashSum  = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
    m_type2Sum = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
    m_type3Sum = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
    m_type4Sum = reader.readPart(5, ByteArray::ByteOrder_LittleEndian);
}
