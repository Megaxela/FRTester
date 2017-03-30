//
// Created by megaxela on 13.03.17.
//

#include "Tools/LoggerProcessor.h"
#include "Tools/StdExtend.h"
#include <algorithm>

LoggerProcessor::LoggerProcessor() :
    m_log(nullptr)
{

}

LoggerProcessor::LoggerProcessor(ParsedLog *log) :
    m_log(log)
{

}

void LoggerProcessor::setParsedLog(ParsedLog *log)
{
    m_log = log;
}

const ParsedLog *LoggerProcessor::parsedLog() const
{
    return m_log;
}

std::vector<LoggerProcessor::ByteArrayData>
LoggerProcessor::getDataExchange(LoggerProcessor::ParseFunctionType parseFunction, LoggerProcessor::DataGetterFunctionType dataGetter)
{
    std::vector<LoggerProcessor::ByteArrayData> result;

    for (uint32_t i = 0; i < m_log->numberOfLines(); ++i)
    {
        auto line = m_log->getLine(i);

        Way way;

        if (parseFunction(line, way))
        {
            result.push_back({line.dateTime,
                              dataGetter(line, way),
                              way});
        }
    }

    return result;
}

std::vector<LoggerProcessor::ByteArrayData>
LoggerProcessor::getSentDataExchange(LoggerProcessor::ParseFunctionType parseFunction,
                                     LoggerProcessor::DataGetterFunctionType dataGetter)
{
    std::vector<LoggerProcessor::ByteArrayData> result;

    for (uint32_t i = 0; i < m_log->numberOfLines(); ++i)
    {
        auto line = m_log->getLine(i);

        Way way;

        if (parseFunction(line, way) && way == Way::Send)
        {
            result.push_back({line.dateTime,
                              dataGetter(line, way),
                              way});
        }
    }

    return result;
}

std::vector<LoggerProcessor::ByteArrayData>
LoggerProcessor::getReceiveDataExchange(LoggerProcessor::ParseFunctionType parseFunction,
                                        LoggerProcessor::DataGetterFunctionType dataGetter)
{
    std::vector<LoggerProcessor::ByteArrayData> result;

    for (uint32_t i = 0; i < m_log->numberOfLines(); ++i)
    {
        auto line = m_log->getLine(i);

        Way way;

        if (parseFunction(line, way) && way == Way::Receive)
        {
            result.push_back({line.dateTime,
                              dataGetter(line, way),
                              way});
        }
    }

    return result;
}

ByteArray LoggerProcessor::defaultDataGetter(const ParsedLog::LineData &data, LoggerProcessor::Way)
{
    std::string messageCopy = data.message;

    messageCopy.erase(0, 2);

    messageCopy.erase(
            std::remove(
                    messageCopy.begin(),
                    messageCopy.end(),
                    ' '
            ),
            messageCopy.end()
    );

    return ByteArray::fromHex(messageCopy);
}

bool LoggerProcessor::defaultDataParser(const ParsedLog::LineData &data, LoggerProcessor::Way &way)
{
    if (data.errorClass != "ExcessLog")
    {
        return false;
    }

    if (!stdex::ends(data.symbol, "read") && !stdex::ends(data.symbol, "write"))
    {
        return false;
    }

    if (stdex::begins(data.message, "->"))
    {
        way = Way::Send;
        return true;
    }

    if (stdex::begins(data.message, "<-"))
    {
        way = Way::Receive;
        return true;
    }

    return false;
}
