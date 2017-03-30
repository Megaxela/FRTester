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

#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <Tools/SystemTools.h>


#ifdef OS_LINUX
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else //OS_WINDOWS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#define Log(msg)                    Logger::i().log(Logger::ErrorClass::Info,       __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, (msg), "Log"      );
#define ExcessLog(msg)              Logger::i().log(Logger::ErrorClass::Info,       __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, (msg), "ExcessLog");
#define Warning(msg)                Logger::i().log(Logger::ErrorClass::Warning,    __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, (msg), "Warning"  );
#define Error(msg)                  Logger::i().log(Logger::ErrorClass::Error,      __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, (msg), "Error"    );
#define Critical(msg)               Logger::i().log(Logger::ErrorClass::Critical,   __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, (msg), "Critical" );

#define LogFunc(msg)                Logger::i().log(Logger::ErrorClass::Info,       __FILE__, __LINE__, std::string(),                   __FUNCTION__, (msg), "Log"      );
#define ExcessLogFunc(msg)          Logger::i().log(Logger::ErrorClass::Info,       __FILE__, __LINE__, std::string(),                   __FUNCTION__, (msg), "ExcessLog");
#define WarningFunc(msg)            Logger::i().log(Logger::ErrorClass::Warning,    __FILE__, __LINE__, std::string(),                   __FUNCTION__, (msg), "Warning"  );
#define ErrorFunc(msg)              Logger::i().log(Logger::ErrorClass::Error,      __FILE__, __LINE__, std::string(),                   __FUNCTION__, (msg), "Error"    );
#define CriticalFunc(msg)           Logger::i().log(Logger::ErrorClass::Critical,   __FILE__, __LINE__, std::string(),                   __FUNCTION__, (msg), "Critical" );

#define LogEx(CLASSNAME, msg)       Logger::i().log(Logger::ErrorClass::Info,       __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, (msg), "Log"      );
#define ExcessLogEx(CLASSNAME, msg) Logger::i().log(Logger::ErrorClass::Info,       __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, (msg), "ExcessLog");
#define WarningEx(CLASSNAME, msg)   Logger::i().log(Logger::ErrorClass::Warning,    __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, (msg), "Warning"  );
#define ErrorEx(CLASSNAME, msg)     Logger::i().log(Logger::ErrorClass::Error,      __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, (msg), "Error"    );
#define CriticalEx(CLASSNAME, msg)  Logger::i().log(Logger::ErrorClass::Critical,   __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, (msg), "Critical" );

#define LogStream()                 Logger::Stream(Logger::ErrorClass::Info,        __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, "Log")
#define ExcessLogStream()           Logger::Stream(Logger::ErrorClass::Info,        __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, "ExcessLog")
#define WarningStream()             Logger::Stream(Logger::ErrorClass::Warning,     __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, "Warning")
#define ErrorStream()               Logger::Stream(Logger::ErrorClass::Error,       __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, "Error")
#define CriticalStream()            Logger::Stream(Logger::ErrorClass::Critical,    __FILE__, __LINE__, SystemTools::getTypeName(*this), __FUNCTION__, "Critical")

#define LogStreamFunc()             Logger::Stream(Logger::ErrorClass::Info,        __FILE__, __LINE__, std::string(),                   __FUNCTION__, "Log")
#define ExcessLogStreamFunc()       Logger::Stream(Logger::ErrorClass::Info,        __FILE__, __LINE__, std::string(),                   __FUNCTION__, "ExcessLog")
#define WarningStreamFunc()         Logger::Stream(Logger::ErrorClass::Warning,     __FILE__, __LINE__, std::string(),                   __FUNCTION__, "Warning")
#define ErrorStreamFunc()           Logger::Stream(Logger::ErrorClass::Error,       __FILE__, __LINE__, std::string(),                   __FUNCTION__, "Error")
#define CriticalStreamFunc()        Logger::Stream(Logger::ErrorClass::Critical,    __FILE__, __LINE__, std::string(),                   __FUNCTION__, "Critical")

#define LogStreamEx(CLASSNAME)      Logger::Stream(Logger::ErrorClass::Info,        __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, "Log")
#define ExcessLogStreamEx(CLASSNAME)Logger::Stream(Logger::ErrorClass::Info,        __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, "Log")
#define WarningStreamEx(CLASSNAME)  Logger::Stream(Logger::ErrorClass::Warning,     __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, "Warning")
#define ErrorStreamEx(CLASSNAME)    Logger::Stream(Logger::ErrorClass::Error,       __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, "Error")
#define CriticalStreamEx(CLASSNAME) Logger::Stream(Logger::ErrorClass::Critical,    __FILE__, __LINE__, CLASSNAME,                       __FUNCTION__, "Critical")

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

    class StreamBuffer : public std::streambuf
    {
    public:
        StreamBuffer();

        void newMessage(ErrorClass errorClass,
                        const char* file,
                        int line,
                        const std::string &classname,
                        const char* function,
                        const char* prefix);

    private:
        StreamBuffer(const StreamBuffer&) = delete;
        StreamBuffer& operator=(const StreamBuffer&) = delete;

    protected:
        int overflow(int __c) override;

    private:

        std::string m_ss;

        ErrorClass m_errorClass;
        const char* m_file;
        int m_line;
        std::string m_classname;
        const char* m_function;
        const char* m_prefix;
    };

    class Stream : public std::ostream
    {
    public:
        Stream(ErrorClass errorClass,
               const char* file,
               int line,
               const std::string& classname,
               const char* function,
               const char* prefix);
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
             const char *file,
             int line,
             const std::string &classname,
             const char *function,
             const std::string &message,
             const char *prefix);

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

    /**
     * @brief Method to set cut alowness.
     * @param cut True - cut filename, False - it will print full filename
     */
    void cutFilename(bool cut);



private:
    struct Message
    {
        Message() :
                errorClass(ErrorClass::Info),
                file(nullptr),
                classname(),
                function(nullptr),
                line(0),
                time(0),
                msg(),
                prefix(nullptr)
        {}

        Message(ErrorClass errorClass,
                const char* file,
                const std::string& classname,
                const char* function,
                int line,
                time_t time,
                std::string msg,
                const char* prefix) :
                errorClass(errorClass),
                file(file),
                classname(classname),
                function(function),
                line(line),
                time(time),
                msg(msg),
                prefix(prefix)
        {}

        Message(const Message& m) :
                errorClass(m.errorClass),
                file(m.file),
                classname(m.classname),
                function(m.function),
                line(m.line),
                time(m.time),
                msg(m.msg),
                prefix(m.prefix)
        {}

        Message& operator=(const Message& message)
        {
            errorClass = message.errorClass;
            file = message.file;
            classname = message.classname;
            function = message.function;
            line = message.line;
            time = message.time;
            msg = message.msg;
            prefix = message.prefix;

            return (*this);
        }

        ErrorClass errorClass;
        const char* file;
        std::string classname;
        const char* function;
        int line;
        time_t time;
        std::string msg;
        const char* prefix;
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
                           const std::string &classname,
                           const char *function,
                           const std::string &message,
                           const std::string &prefix);

    /**
     * @brief Method for expanding s string to expectedSize size with r char.
     * @param s String to be expanded.
     * @param expectedSize Size to expand.
     * @param r Char.
     * @return Expanded string.
     */
    std::string fixSize(const std::string &s, uint32_t expectedSize, char r=' ');

    std::ofstream m_file;

    bool m_working;

    std::queue<Message> m_messages;

    std::mutex m_messagesMutex;

    std::thread m_mainThread;

    std::condition_variable m_cond;

    std::condition_variable m_clearVariable;

    uint32_t m_maxFilenameSize;

    uint32_t m_maxFunctionNameSize;

    std::string m_logFilename;

    bool m_outEnabled;

    ErrorClass m_minErrorClass;

    bool m_cutFilename;
};


#endif //DEVICESERVICE_LOGGER_H
