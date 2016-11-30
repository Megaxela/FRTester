//
// Created by xelam on 30.11.2016.
//

#include <QFile>
#include "include/Tools/Settings.h"

Settings::Settings() :
    m_filename("settings.cfg")
{

}

Settings::~Settings()
{

}

Settings &Settings::instance()
{
    static Settings instance;

    return instance;
}

void Settings::setValue(const QString &name, const QString &value)
{
    m_data[name] = value;
}

QString Settings::getValue(const QString &name, const QString &def) const
{
    if (!m_data.contains(name))
    {
        return def;
    }

    return m_data[name];
}

bool Settings::save()
{
    QFile file(m_filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    for (auto key : m_data)
    {
        QString line = key + "=" + m_data[key] + "\n";
        file.write(QByteArray(line.toStdString().c_str(), line.length()));
    }

    return true;
}

void Settings::setFilename(const QString &name)
{
    m_filename = name;
}

bool Settings::load()
{
    QFile file(m_filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray data = file.readAll();

    QString fileContent = QString::fromUtf8(data);

    QStringList fileLines = fileContent.split('\n');

    m_data.clear();
    for (auto line : fileLines)
    {
        QStringList values = line.split('=');

        if (values.length() != 2)
        {
            return false;
        }

        m_data[values[0]] = values[1];
    }

    file.close();

    return true;
}
