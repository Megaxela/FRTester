//
// Created by megaxela on 02.02.17.
//

#include "ZReportTrigger.h"
#include <Testing/TestEnvironment.h>
#include <Testing/TestLogger.h>
#include <Testing/StaticTriggerFabric.h>

REGISTER_STATIC_TRIGGER(ZReportTrigger)


ZReportTrigger::ZReportTrigger() :
    AbstractTriggerTest(nullptr,
                        "Тригеер закрытия смены.",
                        "Триггер, проверяющий правильность закрытия смены.",
                        true,
                        {{"Password", (uint32_t) 30}}),
    m_success(false),
    m_preError(false),
    m_nonZeroSums(),
    m_countedSums()
{

}

void ZReportTrigger::onPreExecute(const std::string &,
                                  const ByteArray &)
{
    auto password = getValueUInt32("Password");
    m_success = false;
    m_preError = false;

    // Необнуляемые суммы
    m_nonZeroSums = environment()->driver()->getNonZeroSums();

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log("Не удалось получить необнуляемые суммы.");
        m_preError = true;
        return;
    }

    // Денежные регистры
    // Высчитываем общее накопление оплаты через накопление оплат по типу оплаты
    // Приход
    m_countedSums.incomingSum = 0;
    for (auto i = 0; i < 4; ++i)
    {
        auto value = environment()->driver()->currencyRegisterRequest(
                password,
                (uint8_t) (193 + (i * 4))
        );

        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Не удалось получить значение " +
                    std::to_string((193 + (i * 4))) +
                    " регистра. Ошибка # " +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError())
            );
            m_preError = true;
            return;
        }

        m_countedSums.incomingSum += value;
    }

    // Расход
    m_countedSums.consumptionSum = 0;
    for (auto i = 0; i < 4; ++i)
    {
        auto value = environment()->driver()->currencyRegisterRequest(
                password,
                (uint8_t) (194 + (i * 4))
        );

        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Не удалось получить значение " +
                    std::to_string((194 + (i * 4))) +
                    " регистра. Ошибка # " +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError())
            );
            m_preError = true;
            return;
        }

        m_countedSums.consumptionSum += value;
    }

    // Возврат прихода
    m_countedSums.returnIncomingSum = 0;
    for (auto i = 0; i < 4; ++i)
    {
        auto value = environment()->driver()->currencyRegisterRequest(
                password,
                (uint8_t) (195 + (i * 4))
        );

        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Не удалось получить значение " +
                    std::to_string((195 + (i * 4))) +
                    " регистра. Ошибка # " +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError())
            );
            m_preError = true;
            return;
        }

        m_countedSums.returnIncomingSum += value;
    }

    // Возврат расхода
    m_countedSums.returnConsumptionSum = 0;
    for (auto i = 0; i < 4; ++i)
    {
        auto value = environment()->driver()->currencyRegisterRequest(
                password,
                (uint8_t) (196 + (i * 4))
        );

        if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
        {
            environment()->logger()->log(
                    "Не удалось получить значение " +
                    std::to_string((196 + (i * 4))) +
                    " регистра. Ошибка # " +
                    std::to_string((int) environment()->driver()->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString((int) environment()->driver()->getLastError())
            );
            m_preError = true;
            return;
        }

        m_countedSums.returnIncomingSum += value;
    }
}

void ZReportTrigger::onPostExecute()
{
    if (m_preError)
    {
        return;
    }

    auto password = getValueUInt32("Password");
    // Ожидаем окончания печати чека.
    if (!environment()->tools()->waitForPrintingFinished(password, 100000))
    {
        environment()->logger()->log("Не удалось дождаться печати.");
        return;
    }

    // Получаем необнуляемые суммы
    auto newNonZeroSums = environment()->driver()->getNonZeroSums();

    if (environment()->driver()->getLastError() != FRDriver::ErrorCode::NoError)
    {
        environment()->logger()->log(
                "Не удалось получить необнуляемые суммы. Ошибка # " +
                std::to_string((int) environment()->driver()->getLastError()) +
                ' ' +
                FRDriver::Converters::errorToString((int) environment()->driver()->getLastError())
        );

        return;
    }

    // Проверка изменений необнуляемых сумм
    if (newNonZeroSums.incomingSum - m_nonZeroSums.incomingSum != m_countedSums.incomingSum)
    {
        environment()->logger()->log(
                "Неправильное изменение необнуляемой суммы по приходу. Было " +
                std::to_string(m_nonZeroSums.incomingSum) +
                " стало " +
                std::to_string(newNonZeroSums.incomingSum) +
                ". Должно было стать " +
                std::to_string(m_nonZeroSums.incomingSum + m_countedSums.incomingSum)
        );
        return;
    }

    if (newNonZeroSums.consumptionSum - m_nonZeroSums.consumptionSum != m_countedSums.consumptionSum)
    {
        environment()->logger()->log(
                "Неправильное изменение необнуляемой суммы по раходу. Было " +
                std::to_string(m_nonZeroSums.consumptionSum) +
                " стало " +
                std::to_string(newNonZeroSums.consumptionSum) +
                ". Должно было стать " +
                std::to_string(m_nonZeroSums.consumptionSum + m_countedSums.consumptionSum)
        );
        return;
    }

    if (newNonZeroSums.returnIncomingSum - m_nonZeroSums.returnIncomingSum != m_countedSums.returnIncomingSum)
    {
        environment()->logger()->log(
                "Неправильное изменение необнуляемой суммы по возврату прихода. Было " +
                std::to_string(m_nonZeroSums.returnIncomingSum) +
                " стало " +
                std::to_string(newNonZeroSums.returnIncomingSum) +
                ". Должно было стать " +
                std::to_string(m_nonZeroSums.returnIncomingSum + m_countedSums.returnIncomingSum)
        );
        return;
    }

    if (newNonZeroSums.returnConsumptionSum - m_nonZeroSums.returnConsumptionSum != m_countedSums.returnConsumptionSum)
    {
        environment()->logger()->log(
                "Неправильное изменение необнуляемой суммы по возврату расхода. Было " +
                std::to_string(m_nonZeroSums.returnConsumptionSum) +
                " стало " +
                std::to_string(newNonZeroSums.returnConsumptionSum) +
                ". Должно было стать " +
                std::to_string(m_nonZeroSums.returnConsumptionSum + m_countedSums.returnConsumptionSum)
        );
        return;
    }

    m_success = true;
}

bool ZReportTrigger::succeed()
{
    return m_success;
}

bool ZReportTrigger::containsTag(const std::string &tag)
{
    return tag == "shift_close_report";
}
