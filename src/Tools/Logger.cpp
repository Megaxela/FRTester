//
// Created by Megaxela on 23.06.2016.
//

#include "Tools/Logger.h"
#include <iostream>
#include <sstream>
#include <ctime>

Logger::Logger() :
    m_file(),
    m_working(true),
    m_messages(),
    m_messagesMutex(),
    m_mainThread(), // Will be run in constructor
    m_cond(),
    m_logFilename("log.txt"),
    m_outEnabled(true),
    m_minErrorClass(ErrorClass::Info)
{
    // Running main thread.
    m_mainThread = std::thread(&Logger::mainThread, this);

    log(ErrorClass::Info, __FILENAME__, __LINE__, __FUNCTION__, "Logger started", "Log");
}

Logger::~Logger()
{
    Log("Logger stopped.");

    // Waiting until queue will be empty.
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        {
            std::unique_lock<std::mutex> lock(m_messagesMutex);
            if (m_messages.empty())
            {
                break;
            }
        }
    }

    m_working = false;

    m_cond.notify_one();

    if (m_mainThread.joinable())
    {
        m_mainThread.join();
    }
}

Logger &Logger::i()
{
    static Logger l;
    return l;
}

void Logger::log(ErrorClass errorClass,
                 const char *file,
                 int line,
                 const char *function,
                 const std::string &message,
                 const std::string &prefix)
{
    std::unique_lock<std::mutex> lock(m_messagesMutex);

    m_messages.push({
            errorClass,
            file,
            function,
            line,
            time(0),
            message,
            prefix
    });

    m_cond.notify_one();
}

std::string Logger::formString(const char *file,
                               int line,
                               time_t time,
                               const char *function,
                               const std::string &message,
                               const std::string &prefix) const
{
    std::stringstream ss;

    time_t t = time;   // get time now
    struct tm * now = localtime( & t );

    ss << (now->tm_year + 1900) << '-';

    ss.width(2);
    ss.fill('0');
    ss << (now->tm_mon + 1)     << '-';

    ss.width(2);
    ss.fill('0');
    ss <<  now->tm_mday         << ' ';

    ss.width(2);
    ss.fill('0');
    ss <<  now->tm_hour         << ':';

    ss.width(2);
    ss.fill('0');
    ss <<  now->tm_min          << ':';

    ss.width(2);
    ss.fill('0');
    ss <<  now->tm_sec;

    ss << ' ' << file << ':' << line << ' ' << "\t[" << function << "]\t";

    ss << " " << prefix << ":\t\t" << message;

    return ss.str();
}

void Logger::mainThread()
{
    while (m_working)
    {
        std::unique_lock<std::mutex> lock(m_messagesMutex);

        while (m_messages.empty() && m_working)
            m_cond.wait(lock);

        if (!m_logFilename.empty())
        {
            m_file.open("log.txt", std::ios_base::out | std::ios_base::app);
        }

        while (!m_messages.empty())
        {
            std::string result = formString(
                    m_messages.front().file,
                    m_messages.front().line,
                    m_messages.front().time,
                    m_messages.front().function,
                    m_messages.front().msg,
                    m_messages.front().prefix
            );

            if (m_outEnabled)
            {
                if (m_minErrorClass <= m_messages.front().errorClass)
                {
                    switch (m_messages.front().errorClass)
                    {
                        case ErrorClass::Info:
                            std::cout << result << std::endl;
                            break;
                        case ErrorClass::Warning:
                            std::cerr << result << std::endl;
                            break;
                        case ErrorClass::Error:
                            std::cerr << result << std::endl;
                            break;
                        case ErrorClass::Critical:
                            std::cerr << result << std::endl;
                            break;
                        case ErrorClass::None:
                            // What?
                            break;
                        case ErrorClass::Excess:
                            break;
                    }
                }
            }

            if (m_file.is_open())
            {
                m_file << result << '\n';
            }

            m_messages.pop();
        }

        if (m_file.is_open())
        {
            m_file.close();
        }

        m_clearVariable.notify_all();
    }
}

void Logger::setLogFile(const std::string &file)
{
    if (!m_outEnabled && file.empty())
    {
        Error("Can't disable logging with empty log file and turned off console output.");
        return;
    }

    m_logFilename = file;
}

void Logger::setConsoleOutputEnabled(bool enabled)
{
    if (m_logFilename.empty() && !enabled)
    {
        Error("Can't disable logging with empty log file and turned off console output.");
        return;
    }

    m_outEnabled = enabled;
}

void Logger::setLogLevel(Logger::ErrorClass errorClass)
{
    m_minErrorClass = errorClass;
}

void Logger::waitForLogToBeWritten()
{
    std::unique_lock<std::mutex> lock(m_messagesMutex);

    if (m_messages.empty())
        return;

    while (!m_messages.empty())
        m_clearVariable.wait(lock);
}










