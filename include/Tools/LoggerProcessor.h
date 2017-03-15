//
// Created by megaxela on 13.03.17.
//

#ifndef FRTESTER_LOGGERPROCESSOR_H
#define FRTESTER_LOGGERPROCESSOR_H


#include "ParsedLog.h"
#include "ByteArray.h"

class LoggerProcessor
{
public:

    enum class Way
    {
        Send,
        Receive
    };

    typedef std::function<bool(const ParsedLog::LineData& data, Way &way)> ParseFunctionType;

    typedef std::function<ByteArray(const ParsedLog::LineData& data, Way way)> DataGetterFunctionType;

//    typedef std::function<bool(const)

    struct ByteArrayData
    {
        ByteArrayData() :
            time(),
            data(),
            way(Way::Send)
        {}

        ByteArrayData(tm time, ByteArray data, Way way) :
            time(time),
            data(data),
            way(way)
        {}

        tm time;
        ByteArray data;
        Way way;
    };

    struct Session
    {
        Session() :
            begin(),
            end()
        {}

        Session(tm begin, tm end) :
            begin(begin),
            end(end)
        {}

        tm begin;
        tm end;
    };

    /**
     * @brief Конструктор по умолчанию.
     */
    LoggerProcessor();

    /**
     * @brief Конструктор с передачей ссылки на
     * объект лога.
     * @param log Указатель на распаршенный лог.
     */
    LoggerProcessor(ParsedLog* log);

    /**
     * @brief Метод для передачи ссылки на объект
     * распаршенного лога.
     * @param log Указатель на распаршенный лог.
     */
    void setParsedLog(ParsedLog* log);

    /**
     * @brief Метод для получения указателя на используемый
     * распаршенный лог.
     * @return Указатель на распаршенный лог.
     */
    const ParsedLog* parsedLog() const;

    /**
     * @brief Метод для получения всех данных обмена.
     * @param parseFunction Функция для определения сообщения с
     * обменом данных.
     * @return Массив со всеми данными.
     */
    std::vector<ByteArrayData> getDataExchange(LoggerProcessor::ParseFunctionType parseFunction=&LoggerProcessor::defaultDataParser,
                                               LoggerProcessor::DataGetterFunctionType dataGetter=&LoggerProcessor::defaultDataGetter);

    /**
     * @brief Метод для получения всех отправленных данных обмена.
     * @param parseFunction Функция для определения сообщения с
     * обменом данных.
     * @return Массим с отправленными данными.
     */
    std::vector<ByteArrayData> getSentDataExchange(LoggerProcessor::ParseFunctionType parseFunction=&LoggerProcessor::defaultDataParser,
                                                   LoggerProcessor::DataGetterFunctionType dataGetter=&LoggerProcessor::defaultDataGetter);

    /**
     * @brief Метод для получения всех полученных данных обмена.
     * @param parseFunction Функция для определения сообщения с
     * обменом данных.
     * @return Массив с принятыми данными.
     */
    std::vector<ByteArrayData> getReceiveDataExchange(LoggerProcessor::ParseFunctionType parseFunction=&LoggerProcessor::defaultDataParser,
                                                      LoggerProcessor::DataGetterFunctionType dataGetter=&LoggerProcessor::defaultDataGetter);



    /**
     * @brief Метод по-умолчанию для определения сообщений с обменом данных.
     * @param data Данные для обработки.
     * @param way Если это сообщение с обменом данных - то направление обмена.
     * @return Являются ли данные для обработки сообщением с обменом данными.
     */
    static bool defaultDataParser(const ParsedLog::LineData& data, Way &way);

    /**
     * @brief Метод для получения массива байт из сообщения с обменом данных.
     * @param data
     * @return
     */
    static ByteArray defaultDataGetter(const ParsedLog::LineData& data, Way way);

private:

    const ParsedLog* m_log;
};


#endif //FRTESTER_LOGGERPROCESSOR_H
