//
// Created by megaxela on 02.12.16.
//

#include <include/Testing/TestLogger.h>
#include <include/Tools/Time.h>
#include "include/Executor/TestLoggerWaiter.h"

TestLoggerWaiter::TestLoggerWaiter()
{

}

TestLoggerWaiter::~TestLoggerWaiter()
{

}

void TestLoggerWaiter::stopWorking()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_running = false;
}

void TestLoggerWaiter::run()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_running = true;
    }

    while (true)
    {
        bool hasData = false;

        {
            std::unique_lock<std::mutex> lock(m_mutex);

            while (!hasData && m_running)
            {
                lock.unlock();
                if (TestLogger::instance().hasData())
                {
                    hasData = true;
                }

                Time::sleep<std::chrono::milliseconds>(
                        100
                );
                lock.lock();
            }
        }

//        TestLogger::instance().waitForData();

        while (TestLogger::instance().hasData())
        {
            QString s = QString::fromStdString(TestLogger::instance().popMessage());

            emit logReceived(s);
        }

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!m_running)
            {
                break;
            }
        }
    }
}
