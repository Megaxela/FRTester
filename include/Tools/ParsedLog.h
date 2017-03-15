//
// Created by megaxela on 13.03.17.
//

#ifndef FRTESTER_LOGGERPARSER_H
#define FRTESTER_LOGGERPARSER_H

#include <cstdint>
#include <ctime>
#include "Logger.h"

/**
 * @brief Класс, описывающий обработанный лог.
 */
class ParsedLog
{
public:

    struct LineData
    {
        LineData() :
            dateTime(),
            file(),
            line(0),
            symbol(),
            errorClass(),
            message()
        {}

        tm dateTime;
        std::string file;
        uint32_t line;
        std::string symbol;
        std::string errorClass;
        std::string message;
    };

    ParsedLog();

    /**
     * @brief Статический метод для обработки лога.
     * @param stream Входной поток.
     * @return Распаршенный лог.
     */
    static ParsedLog parse(std::ifstream &stream);

    /**
     * @brief Метод для получения количества строк в логе.
     * @return Количество строк.
     */
    uint32_t numberOfLines() const;

    /**
     * @brief Метод для получения количество отправленных байт.
     * @return Количество отправленных байт.
     */
    uint32_t bytesSent() const;

    /**
     * @brief Метод для получения количества полученных байт.
     * @return Количество полученных байт.
     */
    uint32_t bytesReceived() const;

    /**
     * @brief Метод для получения данных строки.
     * @param line Номер строки.
     * @return Данные строки.
     */
    LineData getLine(uint32_t line) const;

private:
    enum class State
    {
        ReadingDateYear,
        ReadingDateMonth,
        ReadingDateDay,
        ReadingTimeHour,
        ReadingTimeMinute,
        ReadingTimeSeconds,
        ReadingFilename,
        ReadingLine,
        ReadingBeforeSymbol,
        ReadingSymbol,
        ReadingBeforeErrorClass,
        ReadingErrorClass,
        ReadingBeforeText,
        ReadingText
    };

    uint32_t m_bytesSent;
    uint32_t m_bytesReceived;
    std::vector<LineData> m_lines;
};


#endif //FRTESTER_LOGGERPARSER_H
