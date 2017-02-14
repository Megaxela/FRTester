//
// Created by megaxela on 03.02.17.
//

#include <include/Tools/Time.h>
#include <include/Testing/TestLogger.h>
#include <include/Tools/Logger.h>
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

TestingTools::Printing TestingTools::enablePrinting(uint32_t password, TestingTools::Printing mode)
{
    // Считываем прошлое состояние
    auto initialPrintValue = m_currentDriver->readTableBin(
            password,
            17,
            1,
            7
    );

    if (m_currentDriver->getLastError() != FRDriver::ErrorCode::NoError)
    {
        m_logger->log("Не удалось получить текущее значение печати чека. Ошибка: #" +
                      std::to_string((int) m_currentDriver->getLastError()) +
                      ' ' +
                      FRDriver::Converters::errorToString((int) m_currentDriver->getLastError()));
        return Printing::Unknown;
    }

    Printing currentValue;

    switch (initialPrintValue)
    {
    case 0: // Печать включена
        currentValue = Printing::Enabled;
        break;
    case 1: // Печать выключена для одного чека
        currentValue = Printing::DisabledForOneCheck;
        break;
    case 2: // Печать выключена
        currentValue = Printing::Disabled;
        break;
    default:
        currentValue = Printing::Unknown;
        break;
    }

    if (currentValue != mode)
    {
        switch (mode)
        {
        case Printing::Unknown:
            Error("В качестве режима был передан неизвестный режим.");
            return Printing::Unknown;
        case Printing::Enabled:
            m_logger->log("Включаем печать чеков.");
            if (!m_currentDriver->writeTable(
                    password,
                    17,
                    1,
                    7,
                    0, // Значение
                    1  // Размер значения
            ))
            {
                m_logger->log(
                        "Не удалось включить печать чеков. Ошибка: #" +
                        std::to_string((int) m_currentDriver->getLastError()) +
                        ' ' +
                        FRDriver::Converters::errorToString((int) m_currentDriver->getLastError())
                );
                return Printing::Unknown;
            }
            break;
        case Printing::DisabledForOneCheck:
            m_logger->log("Отключаем печать чеков для одного документа.");
            if (!m_currentDriver->writeTable(
                    password,
                    17,
                    1,
                    7,
                    1, // Значение
                    1  // Размер значения
            ))
            {
                m_logger->log("Не удалось отключить печать чеков. Ошибка: #" +
                              std::to_string((int) m_currentDriver->getLastError()) +
                              ' ' +
                              FRDriver::Converters::errorToString((int) m_currentDriver->getLastError()));
                return Printing::Unknown;
            }
            break;
        case Printing::Disabled:
            m_logger->log("Отключаем печать чеков.");
            if (!m_currentDriver->writeTable(
                    password,
                    17,
                    1,
                    7,
                    2, // Значение
                    1  // Размер значения
            ))
            {
                m_logger->log("Не удалось отключить печать чеков. Ошибка: #" +
                              std::to_string((int) m_currentDriver->getLastError()) +
                              ' ' +
                              FRDriver::Converters::errorToString((int) m_currentDriver->getLastError()));
                return Printing::Unknown;
            }
            break;
        }
    }

    return currentValue;
}

