//
// Created by Megaxela on 23.06.2016.
//

#include "Tools/Logger.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <algorithm>

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
        m_clearVariable(),
        m_maxFilenameSize(0),
        m_maxFunctionNameSize(0),
        m_logFilename("logs/log.txt"),
        m_outEnabled(true),
        m_minErrorClass(ErrorClass::Info),
        m_cutFilename(true)
{
    // Running main thread.
    m_mainThread = std::thread(&Logger::mainThread, this);

    log(ErrorClass::Info, __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, "Logger started", "Log");
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
                 const std::string& classname,
                 const char *function,
                 const std::string &message,
                 const char *prefix)
{
    std::unique_lock<std::mutex> lock(m_messagesMutex);

    m_messages.push({
                            errorClass,
                            file,
                            classname,
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
                               const std::string& classname,
                               const char *function,
                               const std::string &message,
                               const std::string &prefix)
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

    // Forming filename
    std::string filename;
    {
        std::stringstream filenameSS;

        std::string finishedFilename;
        if (m_cutFilename)
        {
            finishedFilename = (strrchr(file, '/') ? strrchr(file, '/') + 1 : file);
        }
        else
        {
            finishedFilename = file;
        }

        filenameSS << finishedFilename
                   << ':'
                   << line;

        uint32_t len = (uint32_t) filenameSS.str().length();
        if (len > m_maxFilenameSize)
        {
            m_maxFilenameSize = len;
        }

        filename = fixSize(filenameSS.str(), m_maxFilenameSize);
    }

    std::string functionName;
    {
        std::stringstream functionNameSS;

        functionNameSS << "[";

        if (!classname.empty())
        {
            functionNameSS << classname << "::";
        }
        functionNameSS << function
                       << "]";

        uint32_t len = (uint32_t) functionNameSS.str().length();
        if (len > m_maxFunctionNameSize)
        {
            m_maxFunctionNameSize = len;
        }

        functionName = fixSize(functionNameSS.str(), m_maxFunctionNameSize);
    }

    ss << ' ' << filename << ' ' << functionName << ' ';

    ss << ' ' << fixSize(prefix + ": ", 11) << message;

    return ss.str();
}

void Logger::mainThread()
{
    while (m_working)
    {
        {
            std::unique_lock<std::mutex> lock(m_messagesMutex);

            while (m_messages.empty() && m_working)
                m_cond.wait(lock);
        }

        if (!m_logFilename.empty())
        {
            m_file.open(m_logFilename, std::ios_base::out | std::ios_base::app);
        }

        if (m_file.is_open())
        {
            // Checking file size
            uint32_t len = static_cast<uint32_t>(m_file.tellp());
            if (len > 128 * 1024 * 1024)
            {
                m_file.close();
                int i;
                for (i = 0;
                     i < 1024 && SystemTools::Path::fileExists(m_logFilename + "." + std::to_string(i));
                     ++i)
                {

                }

                std::rename(m_logFilename.c_str(), (m_logFilename + "." + std::to_string(i)).c_str());

                m_file.open(m_logFilename, std::ios_base::out | std::ios_base::app);
            }
        }

        Message front = Logger::Message();

        {

            std::unique_lock<std::mutex> lock(m_messagesMutex);
            while (!m_messages.empty())
            {
                {
                    front = m_messages.front();
                    m_messages.pop();
                }

                lock.unlock();

                std::string result = formString(
                        front.file,
                        front.line,
                        front.time,
                        front.classname,
                        front.function,
                        front.msg,
                        front.prefix
                );

                if (m_outEnabled)
                {
                    if (m_minErrorClass <= front.errorClass)
                    {
                        switch (front.errorClass)
                        {
                        case ErrorClass::Info:
                            std::cout << result << std::endl;
                            break;
                        case ErrorClass::Warning:
                            std::cout << result << std::endl;
                            break;
                        case ErrorClass::Error:
                            std::cout << result << std::endl;
                            break;
                        case ErrorClass::Critical:
                            std::cout << result << std::endl;
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

                lock.lock();
            }
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
    std::unique_lock<std::mutex> lock(m_messagesMutex);
    if (!m_outEnabled && file.empty())
    {
        Error("Can't disable logging with empty log file and turned off console output.");
        return;
    }

    m_logFilename = file;
}

void Logger::setConsoleOutputEnabled(bool enabled)
{
    std::unique_lock<std::mutex> lock(m_messagesMutex);
    if (m_logFilename.empty() && !enabled)
    {
        Error("Can't disable logging with empty log file and turned off console output.");
        return;
    }

    m_outEnabled = enabled;
}

void Logger::setLogLevel(Logger::ErrorClass errorClass)
{
    std::unique_lock<std::mutex> lock(m_messagesMutex);
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

std::string Logger::fixSize(const std::string &s, uint32_t expectedSize, char r)
{
    if (expectedSize < s.size())
    {
        return s;
    }

    std::string sCopy = s;

    for (uint32_t i = (uint32_t) s.size(); i <= expectedSize; ++i)
    {
        sCopy += r;
    }

    return sCopy;
}

void Logger::cutFilename(bool cut)
{
    m_cutFilename = cut;
}


Logger::StreamBuffer::StreamBuffer() :
        m_ss(),
        m_errorClass(Logger::ErrorClass::None),
        m_file(nullptr),
        m_line(0),
        m_classname(),
        m_function(nullptr),
        m_prefix(nullptr)
{

}


void Logger::StreamBuffer::newMessage(Logger::ErrorClass errorClass,
                                      const char *file,
                                      int line,
                                      const std::string& classname,
                                      const char *function,
                                      const char *prefix)
{
    m_errorClass = errorClass;
    m_file = file;
    m_line = line;
    m_classname = classname;
    m_function = function;
    m_prefix = prefix;
}

int Logger::StreamBuffer::overflow(int __c)
{
    if (__c == '\n')
    {
        Logger::i().log(m_errorClass, m_file, m_line, m_classname, m_function, m_ss, m_prefix);
        m_ss.clear();
        return traits_type::eof();
    }

    m_ss += (char) __c;

    return __c;
}

static Logger::StreamBuffer streamBuffer;

Logger::Stream::Stream(Logger::ErrorClass errorClass,
                       const char *file,
                       int line,
                       const std::string& classname,
                       const char *function,
                       const char *prefix) :
        std::ostream(&streamBuffer)
{
    streamBuffer.newMessage(errorClass, file, line, classname, function, prefix);
}
