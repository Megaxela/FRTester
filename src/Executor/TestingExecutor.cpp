//
// Created by megaxela on 22.11.16.
//

#include <Testing/TestCore.h>
#include <Tools/Time.h>
#include <DriverHolder.h>
#include <TestDriverHolder.h>
#include <Executor/TestingExecutor.h>

TestingExecutor::TestingExecutor() :
    m_tempTests(),
    m_mutex(),
    m_running(false),
    m_paused(false),
    m_passthroughTesting(false),
    m_notifier(),
    m_timeToWaitForLog(500)
{
    qRegisterMetaType<TestPtr>("TestPtr");
}

TestingExecutor::~TestingExecutor()
{

}

void TestingExecutor::pause()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_paused = true;
}

void TestingExecutor::stop()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_running = false;
    TestCore::instance().interruptTesting();
}

void TestingExecutor::resume()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_paused = false;
}

void TestingExecutor::run()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_running = true;
        m_paused = false;
    }

    TestDriverHolder::driver().setCommandCallback(
            [=](FRDriver::Command command)
            {
                auto tag = TestDriverHolder::driver().getCommandTag(command);

                emit commandCalled(QString::fromStdString(tag));
            }
    );
    emit testingStarted();

    auto tests = getTests();
    auto triggers = TestCore::instance().getActiveTriggers();

    if (tests.empty())
    {
        emit testingFailed("Нечего выполнять. Тесты отсутствуют.");
        std::unique_lock<std::mutex> lock(m_mutex);
        m_running = false;
        m_paused = false;
        return;
    }

    uint32_t index = 0;

    bool lastTestResult;

    while (true)
    {
        // Проверка паузы
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_paused)
            {
                emit testingLogAcquired("Выполнение тестов приостановлено");
                emit testingPaused();
                m_notifier.wait(lock, [=](){ return !m_paused; });

                if (!m_running)
                {
                    emit testingLogAcquired("Тестирование было остановлено.");
                    emit testingStopped();
                    break;
                }
                else
                {
                    emit testingLogAcquired("Тестирование возобновлено.");
                    emit testingResumed();
                }
            }
        }

        if (!DriverHolder::driver().checkConnection())
        {
            // Ждем для того, чтобы вывод логгера был успешно выведен
            Time::sleep<std::chrono::milliseconds>(m_timeToWaitForLog);
            emit testingFailed("Соединение с устройством отсутствует.");
            break;
        }

        emit testingLogAcquired("Восстанавливаем состояние FR.");

        if (!TestCore::instance().restoreFRState())
        {
            emit testingFailed(
                    "Не удалось восстановить начальный режим устройства. Последняя ошибка: " +
                    QString::fromStdString(FRDriver::Converters::errorToString((int) DriverHolder::driver().getLastError()))
            );
            emit testingLogAcquired("Сотояние восстановить не удалось.");
            break;
        }

        emit testingLogAcquired("Выполнение теста №" + QString::number(index + 1) + " " + QString::fromStdString(tests[index]->name()));

        // Для того, чтобы весь предшествующий лог
        // был выведен до теста
        Time::sleep<std::chrono::milliseconds>(100);

        try
        {
            // Запускаем тест
            lastTestResult = tests[index]->execute();
        }
        catch (const std::exception &e)
        {
            // Ждем для того, чтобы вывод логгера был успешно выведен
            Time::sleep<std::chrono::milliseconds>(m_timeToWaitForLog);
            lastTestResult = false;
            emit testingErrorAcquired("Во время выполнения теста было вызвано исключение: " + QString(e.what()));
        }

        // Для того, чтобы весь вывод теста был успешно
        // выведен в лог
        Time::sleep<std::chrono::milliseconds>(100);

        emit testResultAcquired(tests[index], lastTestResult);

        // Проверка тригеров
        if (TestCore::instance().hasFailedTriggers())
        {
            auto failedTriggers = TestCore::instance().getFailedTriggers();

            bool needPause = false;
            for (auto trigger : failedTriggers)
            {
                if (trigger->isCritical())
                {
                    emit testingLogAcquired(
                            "Критичный триггер \"" +
                            QString::fromStdString(trigger->name()) +
                            "\" обнаружил ошибку. По завершению проверки остальных триггеров тестирование будет приостановлено."
                    );
                    needPause = true;
                }

                emit triggerFailAcquired(trigger);
            }

            TestCore::instance().clearFailedTriggers();

            if (needPause)
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_paused = true;
            }
        }

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!lastTestResult && !m_passthroughTesting)
            {
                Time::sleep<std::chrono::milliseconds>(m_timeToWaitForLog);
                emit testingStopped();
            }
        }

        // Проверка работоспособности
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!m_running)
            {
                Time::sleep<std::chrono::milliseconds>(m_timeToWaitForLog);
                emit testingStopped();
                break;
            }
        }

        ++index;
        if (index >= tests.size())
        {
            Time::sleep<std::chrono::milliseconds>(m_timeToWaitForLog);
            emit testingFinished();
            break;
        }
    }

    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_running = false;
        m_paused = false;
    }
}

bool TestingExecutor::isTestingRunning()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    return m_running;
}

bool TestingExecutor::isPaused()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    return m_paused;
}

void TestingExecutor::setTestsToRun(const std::vector<TestPtr> &tests)
{
    m_tempTests = tests;
    start();
}

std::vector<TestPtr> TestingExecutor::getTests()
{
    if (m_tempTests.size() > 0)
    {
        auto tmp = m_tempTests;
        m_tempTests.clear();

        return tmp;
    }
    else
    {
        return TestCore::instance().getActiveTests();
    }
}



