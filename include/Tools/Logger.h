//
// Created by Megaxela on 23.06.2016.
//

#ifndef DEVICESERVICE_LOGGER_H
#define DEVICESERVICE_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Platform.h"

#ifdef OS_WINDOWS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif
#ifdef OS_LINUX
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define Log(msg)        Logger::i().log(Logger::ErrorClass::Info,       __FILENAME__, __LINE__, __FUNCTION__, (msg), "Log"  );
#define Warning(msg)    Logger::i().log(Logger::ErrorClass::Warning,    __FILENAME__, __LINE__, __FUNCTION__, (msg), "Warning");
#define Error(msg)      Logger::i().log(Logger::ErrorClass::Error,      __FILENAME__, __LINE__, __FUNCTION__, (msg), "Error");
#define Critical(msg)   Logger::i().log(Logger::ErrorClass::Critical,   __FILENAME__, __LINE__, __FUNCTION__, (msg), "Critical");

#define DebugLog(msg) Logger::i().log(Logger::ErrorClass::Info,       __FILENAME__, __LINE__, __FUNCTION__, (msg), "Log"  );
#define DebugError(msg) Logger::i().log(Logger::ErrorClass::Error,      __FILENAME__, __LINE__, __FUNCTION__, (msg), "Error");

#define ExcessLog(msg) Logger::i().log(Logger::ErrorClass::Excess,      __FILENAME__, __LINE__, __FUNCTION__, (msg), "ShadowLog");
/**
 * @brief Singleton class to control logging process.
 */
class Logger
{
public:
    enum class ErrorClass
    {
        Excess,
        Info,
        Warning,
        Error,
        Critical,
        None,
    };

    /**
     * @brief Destructor.
     */
    ~Logger();

    /**
     * @brief Singleton instance getter.
     * @return Instance of Logger.
     */
    static Logger& i();

    /**
     * @brief Main log method.
     * @param stream Log string will be printed to this stream.
     * @param file Filename of log call
     * @param line Line number of log call
     * @param function Function name of log call
     * @param message Log message
     * @param prefix Log prefix
     */
    void log(ErrorClass errorClass,
             const char* file,
             int line,
             const char* function,
             const std::string &message,
             const std::string &prefix);

    /**
     * @brief Method to set log file. If empty - there is no log.
     * @param file Log file path.
     */
    void setLogFile(const std::string &file);

    /**
     * @brief Method to set console output.
     * It can't be disabled with log file in the same time.
     * Use setLogLevel to disable logging.
     */
    void setConsoleOutputEnabled(bool enabled);

    /**
     * @brief Method to set log level. All logs that
     * have lower level will be ignored. To ignore all
     * logs use ErrorClass::None.
     */
    void setLogLevel(ErrorClass errorClass);

    /**
     * @brief Waiting until queue will be empty.
     */
    void waitForLogToBeWritten();

private:
    struct Message
    {
        ErrorClass errorClass;
        const char* file;
        const char* function;
        int line;
        time_t time;
        std::string msg;
        std::string prefix;
    };

    /**
     * @brief Private constructor, because it's singleton.
     */
    Logger();

    /**
     * @brief Main logger thread.
     */
    void mainThread();

    /**
     * @brief Method for forming formatted message from some args.
     * @param file Filename where was this log message posted.
     * @param line Line of that filename where was this log message posted.
     * @param time Time of message.
     * @param function Function name where is message was posted.
     * @param message Actual log message.
     * @param prefix Log message prefix.
     * @return Result string.
     */
    std::string formString(const char *file,
                           int line,
                           time_t time,
                           const char *function,
                           const std::string &message,
                           const std::string &prefix) const;

    std::ofstream m_file;

    bool m_working;

    std::queue<Message> m_messages;

    std::mutex m_messagesMutex;

    std::thread m_mainThread;

    std::condition_variable m_cond;

    std::condition_variable m_clearVariable;

    std::string m_logFilename;

    bool m_outEnabled;

    ErrorClass m_minErrorClass;
};


#endif //DEVICESERVICE_LOGGER_H
