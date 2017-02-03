//
// Created by megaxela on 03.02.17.
//

#include <include/Tools/Time.h>
#include <include/Testing/TestLogger.h>
#include "include/Testing/TestingTools.h"

TestingTools::TestingTools(TestDriver *testDriver, TestLogger* logger) :
    m_testDriver(testDriver),
    m_currentDriver(testDriver),
    m_logger(logger)
{

}

TestingTools::~TestingTools()
{

}

void TestingTools::enableTesting(bool enabled)
{
    if (enabled)
    {
        m_currentDriver = m_testDriver;
    }
    else
    {
        m_currentDriver = m_testDriver->pureDriver();
    }
}

bool TestingTools::waitForPrintingFinished(uint32_t password, uint32_t timeout)
{
    Time::time_t start = Time::get<std::chrono::milliseconds>();

    m_logger->log("Ожидание окончания печати чека.");
    while (true)
    {
        auto state = m_currentDriver->shortStateRequest(password);

        if (m_currentDriver->getLastError() != FRDriver::ErrorCode::NoError)
        {
            m_logger->log(
                    "Не удалось выполнить короткий запрос состояния. Ошибка #" +
                    std::to_string((int) m_currentDriver->getLastError()) +
                    ' ' +
                    FRDriver::Converters::errorToString((int) m_currentDriver->getLastError())
            );
            return false;
        }

        if (state.posSubMode == 0)
        {
            m_logger->log("ФР завершил печать.");
            return true;
        }

        if (Time::get<std::chrono::milliseconds>() - start >= timeout && timeout != 0)
        {
            m_logger->log(
                    "Таймаут ожидания печати. Печать не завершилась за " +
                    std::to_string(timeout) + "мс."
            );
            return false;
        }
    }
}
