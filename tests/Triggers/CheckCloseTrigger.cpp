//
// Created by megaxela on 10.01.17.
//

#include "CheckCloseTrigger.h"
#include <algorithm>
#include <include/Tools/ByteArrayReader.h>
#include <Testing/TestEnvironment.h>
#include <Testing/TestLogger.h>

CheckCloseTrigger::CheckCloseTrigger(TestEnvironment *environment) :
    AbstractTriggerTest(
            environment,
            "Триггер закрытия чека",
            "Триггер контроллирующий правильность оперирования"
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