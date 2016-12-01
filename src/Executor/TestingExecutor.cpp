//
// Created by megaxela on 22.11.16.
//

#include <include/Testing/TestCore.h>
#include "include/Executor/TestingExecutor.h"

TestingExecutor::TestingExecutor() :
    m_paused(false),
    m_running(false)
{

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

    emit testingStarted();

    emit testingLogAcquired("Тестирование началось");

    auto tests = TestCore::instance().getTests();
    auto triggers = TestCore::instance().getTriggers();

    int index = 0;

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

        if (!TestCore::instance().restoreFRState())
        {
            emit testingFailed("Не удалось восстановить начальный режим устройства.");
            break;
        }

        emit testingLogAcquired("Выполнение теста №" + QString::number(index + 1) + " " + QString::fromStdString(tests[index]->name()));

        try
        {
            // Запускаем тест
            lastTestResult = tests[index]->execute();

            emit testResultAcquired(tests[index], lastTestResult);
        }
        catch (const std::exception &e)
        {
            lastTestResult = false;
            emit testingLogAcquired("Во время выполнения теста было вызвано исключение: " + QString(e.what()));
            emit testResultAcquired(tests[index], false);
        }

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
                emit testingLogAcquired("Тестирование будет прервано, поскольку тест не прошел успешно.");
                emit testingStopped();
            }
        }

        // Проверка работоспособности
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!m_running)
            {
                emit testingLogAcquired("Тестирование остановлено.");
                emit testingStopped();
                break;
            }
        }
    }

    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_running = false;
        m_paused = false;
    }

    emit testingLogAcquired("Тестирование завершено");
    emit testingFinished();
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

