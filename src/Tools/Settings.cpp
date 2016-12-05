//
// Created by xelam on 30.11.2016.
//

#include <include/Tools/Logger.h>
#include "include/Tools/Settings.h"

const std::string Settings::settingsFilename = "settings.ini";

Settings::Settings()
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
    std::ofstream file(Settings::settingsFilename);

    if (!file.is_open())
    {
        Error("Не удалось открыть файл с настройками.");
        return false;
    }

    for (auto key : m_data)
    {
        file << key.first << '=' << key.second << '\n';
    }

    file.close();

    Log("Настройки сохранены.");

    return true;
}

bool Settings::load()
{
    std::ifstream file(Settings::settingsFilename);

    if (!file.is_open())
    {
        Error("Не удалось открыть файл с настройками.");
        return false;
    }

    std::streampos size = file.tellg();
    if (size > 10485760)
    {
        Error("Не удалось прочитать настройки. Файл с настроками > чем 10MB.");
        file.close();
        return false;
    }

    char buffer[(uint64_t) size];

    file.seekg(0, std::ios::beg);

    file.read(buffer, size);

    file.close();

    m_data.clear();
    std::string name;
    std::string value;
    int mode = 0; // 0 - name, 1 - value
    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == '=')
        {
            if (mode != 1)
            {
                mode = 1;
                value.clear();
            }
            else
            {
                value += buffer[i];
            }
        }
        else if (buffer[i] == '\n')
        {
            if (mode == 1)
            {
                m_data[name] = value;
                name.clear();
            }
        }
        else
        {
            if (mode == 0)
            {
                name += buffer[i];
            }
            else
            {
                value += buffer[i];
            }
        }
    }

    Log("Настройки загружены.");

    return true;
}
