//
// Created by xelam on 30.11.2016.
//

#include <include/Tools/Logger.h>
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

void Settings::setValue(const std::string &name, const std::string &value)
{
    m_data[name] = value;
}

std::string Settings::getValue(const std::string &name, const std::string &def)
{
    for (auto item : m_data)
    {
        if (item.first == name)
        {
            return item.second;
        }
    }

    return def;
}

bool Settings::save()
{
    Error("Not reimplemented yet.");

    return true;
}

void Settings::setFilename(const std::string &name)
{
    m_filename = name;
}

bool Settings::load()
{
    Error("Not reimplemented yet.");

    return true;
}
