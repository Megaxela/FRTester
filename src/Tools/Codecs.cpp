//
// Created by megaxela on 13.12.16.
//

#include "include/Tools/Codecs.h"
#include <QTextCodec>

Codecs::Codecs()
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
    }

    if (!m_codecs.contains(to))
    {
        m_codecs[to] = QTextCodec::codecForName(to.c_str());
    }

//    data = m_codecs[from]->toUnicode(data);
}

QString Codecs::convert(std::string from, QByteArray data)
{
    if (!m_codecs.contains(from))
    {
        m_codecs[from] = QTextCodec::codecForName(from.c_str());
    }

    return m_codecs[from]->toUnicode(data);
}
