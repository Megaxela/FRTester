//
// Created by megaxela on 02.12.16.
//

#include "include/Testing/TestLogger.h"

TestLogger::TestLogger() :
    m_notifier(),
    m_mutex(),
    m_finished(false),
    m_messages()
{

}

TestLogger::~TestLogger()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_finished = true;
    }

    m_notifier.notify_all();
}

TestLogger &TestLogger::instance()
{
    static TestLogger logger;
    return logger;
}

void TestLogger::log(const std::string &string)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_messages.push(string);
    m_notifier.notify_all();
}

void TestLogger::waitForData()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_notifier.wait(lock, [=]()
    {
        return !m_messages.empty() || m_finished;
    });
}

std::string TestLogger::popMessage()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    std::string str = m_messages.front();
    m_messages.pop();
    return str;
}

bool TestLogger::hasData() const
{
    std::unique_lock<std::mutex> lock(m_mutex);

    return !m_messages.empty();
}


