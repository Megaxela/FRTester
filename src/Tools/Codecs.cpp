//
// Created by megaxela on 13.12.16.
//

#include "include/Tools/Codecs.h"
#include <QTextCodec>
#include <include/Tools/Logger.h>

Codecs::Codecs() :
    m_codecs()
{

}

Codecs::~Codecs()
{

}

Codecs &Codecs::instance()
{
    static Codecs codecs;
    return codecs;
}

QByteArray Codecs::convert(std::string from, std::string to, QByteArray data)
{
    if (!m_codecs.contains(from))
    {
        m_codecs[from] = QTextCodec::codecForName(from.c_str());
        if (m_codecs[from] == nullptr)
        {
            Critical("Не удалось получить кодек под названием \"" + from + "\".");
            return QByteArray();
        }
    }

    if (!m_codecs.contains(to))
    {
        m_codecs[to] = QTextCodec::codecForName(to.c_str());
        if (m_codecs[to] == nullptr)
        {
            Critical("Не удалось получить кодек под названием \"" + to + "\".");
            return QByteArray();
        }
    }

    auto tmp = m_codecs[from]->toUnicode(data);

    return m_codecs[to]->fromUnicode(tmp);
}

QString Codecs::convert(std::string from, QByteArray data)
{
    if (!m_codecs.contains(from))
    {
        m_codecs[from] = QTextCodec::codecForName(from.c_str());
    }

    return m_codecs[from]->toUnicode(data);
}

QByteArray Codecs::convertTo(std::string to, QString data)
{
    if (!m_codecs.contains(to))
    {
        m_codecs[to] = QTextCodec::codecForName(to.c_str());
    }

    return m_codecs[to]->fromUnicode(data);
}
