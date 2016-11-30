//
// Created by megaxela on 21.11.16.
//

#include <include/Testing/TestCore.h>
#include <include/DriverHolder.h>
#include <tests/Tests/CycleTest.h>
#include <Tools/Logger.h>

TestCore::TestCore()
{

}

TestCore::~TestCore()
{

}

TestCore &TestCore::instance()
{
    static TestCore core;

    return core;
}

void TestCore::updateDatabase()
{
    m_tests.clear();
    m_triggers.clear();

    m_tests.push_back(std::make_shared<CycleTest>(&m_testDriver));

}

void TestCore::getTriggers(const std::string &tag,
                           std::vector<TriggerTestPtr> &v)
{
    for (auto trigger : m_triggers)
    {
        if (trigger->containsTag(tag))
        {
            v.push_back(trigger);
        }
    }
}

void TestCore::runTests()
{
    m_triggerFailed = false;

    m_allFailedTriggers.clear();
    m_failedTriggers.clear();

    for (auto test : m_tests)
    {
        // Пытаемся восстановить состояние ФР
        if (!restoreFRState())
        {
            Critical("Не удалось восстановить состояние ФР. Прерываем тестирование.");
            return;
        }

        // Запускаем тест
        if (!test->execute())
        {
            Critical("Тест \"" +
                     test->name() +
                     "\" прошел неудачно. Останавливаем тестирование.");
            return;
        }

        // Проверяем как там с тригерами
        if (m_triggerFailed)
        {
            m_triggerFailed = false;

            for (auto trigger : m_failedTriggers)
            {
                m_allFailedTriggers.push_back(trigger);
            }

            for (auto trigger : m_failedTriggers)
            {
                if (trigger->isCritical())
                {
                    Critical("Критичный тригер \"" +
                             trigger->name() +
                             "\" обнаружил ошибку. Останавливаем тестирование.");
                    return;
                }

                Error("Некритичный триггер \"" +
                      trigger->name() +
                      "\" обнаружил ошибку.");
                return;
            }

            m_failedTriggers.clear();
        }
    }
}

bool TestCore::restoreFRState()
{
    auto state = DriverHolder::driver().fullStateRequest(30);
    if (state.posMode == 8) // Открытый документ
    {
        // Отменяем документ
        if (!DriverHolder::driver().cancelCheck(30))
        {
            Error("Не удалось отменить открытый чек.");
            return false;
        }
    }
    else if (state.posMode == 2) // Открытая смена
    {
        // Закрываем смену
        if (!DriverHolder::driver().shiftCloseReport(30))
        {
            Error("Не удалось закрыть смену.");
            return false;
        }
    }

    return true;
}

void TestCore::triggerMistaken(TriggerTestPtr trigger)
{
    m_triggerFailed = true;
    m_failedTriggers.push_back(trigger);
}

std::vector<TestPtr> TestCore::getTests()
{
    return m_tests;
}

std::vector<TriggerTestPtr> TestCore::getTriggers()
{
    return m_triggers;
}
