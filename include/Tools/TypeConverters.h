//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_TYPECONVERTERS_H
#define FRTESTER_TYPECONVERTERS_H

#include <cstdint>
#include <QString>

namespace TypeConverters
{
    uint64_t toUint64(QString q);
}

struct ConvertException : std::exception
{
    ConvertException(const std::string& s) :
            m_s(s)
    {

    }

    const char *what() const noexcept override
    {
        return m_s.c_str();
    }

    std::string m_s;
};

#endif //FRTESTER_TYPECONVERTERS_H
