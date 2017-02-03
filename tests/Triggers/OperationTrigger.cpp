//
// Created by megaxela on 09.01.17.
//

#include "OperationTrigger.h"
#include <algorithm>
#include <include/Tools/ByteArrayReader.h>
#include <include/Testing/FROperations.h>
#include "Testing/TestEnvironment.h"
#include "Testing/TestLogger.h"

OperationTrigger::OperationTrigger(TestEnvironment *environment) :
        AbstractTriggerTest(environment,
                            "Триггер операций",
                            "Триггер, контроллирующий правильность выполнения "
                            "операций.",
                            true,
                            {{"Password", (uint32_t) 30}}),
        m_tags({"sell", "buy", "return_sell", "return_buy"}),
        m_success(false)
{

}

void OperationTrigger::onPreExecute(const std::string &realTag,
                                    const ByteArray &arguments)
{
    auto password = getValueUInt32("Password");
    m_success = false;
    parseArguments(arguments);
    m_tag = realTag;

    uint8_t registerIndex = (uint8_t) ((m_dep - 1) * 4 + tagToIndex(realTag));
    // Получение накопления операции по отделам
    m_moneyRegistersOperationsByDepartment = environment()->driver()->currencyRegisterRequest(
            password,
            registerIndex
    );

    // Получения количества операций по отделам
    m_operatingRegisterOperations = environment()->driver()->operatingRegisterRequest(
            password,
            registerIndex
    );

    // Попытка получения подытога
    m_previousCheckResult = environment()->driver()->checkResult(
            password
    );
}

void OperationTrigger::onPostExecute()
{
    auto password = getValueUInt32("Password");

    uint8_t registerIndex = (uint8_t) ((m_dep - 1) * 4 + tagToIndex(m_tag));

    auto currencyAfter = environment()->driver()->currencyRegisterRequest(
            password,
            registerIndex
    );
    auto currencyDifference = currencyAfter - m_moneyRegistersOperationsByDepartment;

    auto calculatedDifference = FROperations::smartRound(
            m_value * (m_count * 0.001)
    );

    if (currencyDifference != calculatedDifference)
    {
        environment()->logger()->log(
                "Расхождения в денежных регистрах: " +
                std::to_string(currencyDifference) +
                " != " +
                std::to_string(calculatedDifference)
        );
        return;
    }

    auto operationsAfter = environment()->driver()->operatingRegisterRequest(
            password,
            registerIndex
    );

    auto operationsDifference = operationsAfter - m_operatingRegisterOperations;

    if (operationsDifference != 1)
    {
        environment()->logger()->log(
                "Расхождения в операционных регистрах: " +
                std::to_string(operationsDifference) +
                " != 1"
        );
        return;
    }

    auto checkResultAfter = environment()->driver()->checkResult(password);

    auto checkResultDifference = checkResultAfter - m_previousCheckResult;

    if (checkResultDifference != calculatedDifference)
    {
        environment()->logger()->log(
                "Расхождение в подытоге чека: " +
                std::to_string(checkResultDifference) +
                " != " +
                std::to_string(calculatedDifference)
        );

        return;
    }

    m_success = true;
}

bool OperationTrigger::succeed()
{
    return m_success;
}

bool OperationTrigger::containsTag(const std::string &tag)
{
    return std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end();
}

void OperationTrigger::parseArguments(const ByteArray &arguments)
{
    ByteArrayReader reader(arguments);
    m_pwd   = reader.read<uint32_t>(   ByteArray::ByteOrder_LittleEndian);
    m_count = reader.readPart      (5, ByteArray::ByteOrder_LittleEndian);
    m_value = reader.readPart      (5, ByteArray::ByteOrder_LittleEndian);
    m_dep   = reader.read<uint8_t> (   ByteArray::ByteOrder_LittleEndian);

    if (m_dep == 0)
    {
        m_dep = 1;
    }
}

uint32_t OperationTrigger::tagToIndex(const std::string &tag)
{
    if (tag == "sell")
    {
        return 0;
    }
    else if (tag == "buy")
    {
        return 1;
    }
    else if (tag == "return_sell")
    {
        return 2;
    }
    else if (tag == "return_buy")
    {
        return 3;
    }
    else
    {
        Log("Не могу получить индекс для текущго тега: " + tag);
        return 0;
    }
}
