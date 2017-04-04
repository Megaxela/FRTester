//
// Created by megaxela on 13.03.17.
//

#include "Tools/ParsedLog.h"

ParsedLog::ParsedLog() :
    m_bytesSent(0),
    m_bytesReceived(0),
    m_lines()
{

}

ParsedLog ParsedLog::parse(std::ifstream &stream)
{
    State state = State::ReadingDateYear;
    ParsedLog log;

    char character;
    LineData lineData;
    std::stringstream ss;
    while (!stream.eof())
    {
        stream.read(&character, 1);

        switch (state)
        {
        case State::ReadingDateYear:
            if (character == '-')
            {
                state = State::ReadingDateMonth;
            }

            ss << character;

            break;
        case State::ReadingDateMonth:
            if (character == '-')
            {
                state = State::ReadingDateDay;
            }

            ss << character;

            break;
        case State::ReadingDateDay:
            if(character == ' ')
            {
                state = State::ReadingTimeHour;
            }

            ss << character;

            break;
        case State::ReadingTimeHour:
            if (character == ':' )
            {
                state = State::ReadingTimeMinute;
            }

            ss << character;

            break;
        case State::ReadingTimeMinute:
            if (character == ':')
            {
                state = State::ReadingTimeSeconds;
            }

            ss << character;

            break;
        case State::ReadingTimeSeconds:
            if (character == ' ')
            {
#ifdef OS_LINUX
                //todo: Добавить кроссплатформенное преобразование
                strptime(ss.str().c_str(), "%Y-%m-%dT %H:%M:%S", &lineData.dateTime);
#endif
                ss.clear();
                state = State::ReadingFilename;
            }
            else
            {
                ss << character;
            }

            break;
        case State::ReadingFilename:
            if (character == ':')
            {
                lineData.file = ss.str();
                ss.clear();
                state = State::ReadingLine;
            }
            else
            {
                ss << character;
            }
            break;
        case State::ReadingLine:
            if (character == ' ')
            {
                lineData.line = static_cast<uint32_t>(std::stoi(ss.str()));
                ss.clear();
                state = State::ReadingBeforeSymbol;
            }
            else
            {
                ss << character;
            }
            break;
        case State::ReadingBeforeSymbol:
            if (character != ' ')
            {
                ss << character;
                state = State::ReadingSymbol;
            }
            break;
        case State::ReadingSymbol:
            if (character == ']')
            {
                lineData.symbol = ss.str();
                ss.clear();
                state = State::ReadingBeforeErrorClass;
            }
            else
            {
                if (character != '[')
                {
                    ss << character;
                }
            }
            break;
        case State::ReadingBeforeErrorClass:
            if (character != ' ')
            {
                ss << character;
                state = State::ReadingErrorClass;
            }
            break;
        case State::ReadingErrorClass:
            if (character == ':')
            {
                lineData.errorClass = ss.str();
                ss.clear();
                state = State::ReadingBeforeText;
            }
            else
            {
                ss << character;
            }
            break;
        case State::ReadingBeforeText:
            if (character != ' ')
            {
                ss << character;
                state = State::ReadingText;
            }

            break;
        case State::ReadingText:
            if (character == '\n')
            {
                lineData.message = ss.str();
                ss.clear();

                log.m_lines.push_back(lineData);

                state = State::ReadingDateYear;
            }
            else
            {
                ss << character;
            }

            break;
        }
    }

    return log;
}

uint32_t ParsedLog::numberOfLines() const
{
    return static_cast<uint32_t>(m_lines.size());
}

uint32_t ParsedLog::bytesSent() const
{
    return m_bytesSent;
}

uint32_t ParsedLog::bytesReceived() const
{
    return m_bytesReceived;
}

ParsedLog::LineData ParsedLog::getLine(uint32_t line) const
{
    return m_lines[line];
}
