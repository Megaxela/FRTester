//
// Created by megaxela on 21.11.16.
//

#include "include/Tools/TypeConverters.h"
#include <QString>

uint64_t TypeConverters::toUint64(QString s)
{
    bool ok = false;

    uint64_t n = s.toULongLong(&ok);

    if (!ok)
    {
        throw ConvertException(s.toStdString() + " не может быть преобразовано в число.");
    }

    return n;
}