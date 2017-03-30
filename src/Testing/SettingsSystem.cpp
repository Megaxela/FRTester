//
// Created by megaxela on 02.02.17.
//

#include <fstream>
#include <Tools/Logger.h>
#include <Testing/AbstractTest.h>
#include "Testing/SettingsSystem.h"
#include "libraries/json.hpp"

#define SETTINGS_FILE "settings.json"

using json = nlohmann::json ;

const std::string SettingsSystem::ConnectionIPAddress  = "connection_ip_address";
const std::string SettingsSystem::ConnectionCOMPort = "connection_com_port";
const std::string SettingsSystem::ConnectionCOMSpeed = "connection_com_speed";
const std::string SettingsSystem::ConnectionIPPort = "connection_ip_port";
const std::string SettingsSystem::GlobalTabSelected = "global_tab_selected";
const std::string SettingsSystem::TestsSharedTestsPath = "tests_tests_path";
const std::string SettingsSystem::TestsSharedTriggersPath = "tests_triggers_path";
const std::string SettingsSystem::TestsManualTestsPath = "tests_manual_tests_path";

SettingsSystem::~SettingsSystem()
{
    saveData();
}

SettingsSystem &SettingsSystem::instance()
{
    static SettingsSystem inst;
    return inst;
}

void SettingsSystem::setTestVariable(AbstractTest* test,
                                     const std::string& name,
                                     const DataValue &value)
{
    auto hash = m_hash(test->name() + test->description());

    auto findIterator = m_testVariables.find(hash);
    if (findIterator == m_testVariables.end())
    {
        m_testVariables[hash] = std::map<std::string, DataValue>();
        m_testVariables[hash][name] = value;
    }
    else
    {
        findIterator->second[name] = value;
    }
}

DataValue SettingsSystem::getTestVariable(AbstractTest* test,
                                                        const std::string& name,
                                                        bool* ok)
{
    auto hash = m_hash(test->name() + test->description());

    if (m_testVariables.find(hash) == m_testVariables.end())
    {
        if (ok)
        {
            *ok = false;
        }
        return DataValue();
    }

    if (m_testVariables[hash].find(name) == m_testVariables[hash].end())
    {
        if (ok)
        {
            *ok = false;
        }
        return DataValue();
    }

    if (ok)
    {
        *ok = true;
    }
    return m_testVariables[hash][name];
}

SettingsSystem::SettingsSystem() :
    m_values(),
    m_testVariables(),
    m_triggerVariables(),
    m_hash()
{
    loadData();
}

std::string SettingsSystem::getValue(const std::string &name, const std::string& defaultValue)
{
    if (m_values.find(name) == m_values.end())
    {
        return defaultValue;
    }

    return m_values[name];
}

void SettingsSystem::setValue(const std::string &name, const std::string &value)
{
    m_values[name] = value;
    saveData();
}

void SettingsSystem::loadData()
{
    Log("Загрузка сохраненных значений.");
    // Loading file with data
    if (!SystemTools::Path::fileExists(SETTINGS_FILE))
    {
        Warning("Не существует файла сохранений.");
        return;
    }

    std::ifstream file(SETTINGS_FILE);

    if (!file)
    {
        Warning("Не удалось открыть существующий файл сохранений.");
        return;
    }

    json root = json::parse(file);

    if (root.empty())
    {
        Warning("Не удалось распарсить файл сохранений.");
        return;
    }

    // Загрузка настроек
    json settingsDict = root["settings"];
    for (json::iterator key = settingsDict.begin(); key != settingsDict.end(); ++key)
    {
        m_values[key.key()] = key.value();
    }

    // Загрузка значений
    json testVariablesDict = root["test_variables"];
    for (json::iterator test = testVariablesDict.begin();
         test != testVariablesDict.end();
         ++test)
    {
        auto longKey = std::stoul(test.key());
        m_testVariables[longKey] = std::map<std::string, DataValue>();
        json value = test.value();
        for (json::iterator variable = value.begin();
             variable  != value.end();
             ++variable)
        {
            DataValue v;
            DataValue::from_json(variable.value(), v);
            m_testVariables[longKey][variable.key()] = v;
        }
    }

    testVariablesDict = root["trigger_variables"];
    for (json::iterator test = testVariablesDict.begin(); test != testVariablesDict.end(); ++test)
    {
        auto longKey = std::stoul(test.key());
        m_triggerVariables[longKey] = std::map<std::string, DataValue>();
        json value = test.value();
        for (json::iterator variable = value.begin(); variable  != value.end(); ++variable)
        {
            DataValue v;
            DataValue::from_json(variable.value(), v);
            m_triggerVariables[longKey][variable.key()] = v;
        }
    }
}

void SettingsSystem::saveData()
{
    ExcessLog("Сохранение значений.");
    json root;
    root["settings"] = std::map<std::string, std::string>();
    root["test_variables"] = std::map<std::string, json>();
    root["trigger_variables"] = std::map<std::string, json>();

    // Запись настроек
    for (auto& value : m_values)
    {
        root["settings"][value.first] = value.second;
    }

    // Запись значений переменных
    for (auto& test : m_testVariables)
    {
        json variables;
        for (auto& variable : test.second)
        {
            json j;
            DataValue::to_json(j, variable.second);
            variables[variable.first] = j;
        }

        root["test_variables"][std::to_string(test.first)] = variables;
    }

    // Проходимся по триггерам
    for (auto& test : m_triggerVariables)
    {
        json variables;
        for (auto& variable : test.second)
        {
            json j;

            DataValue::to_json(j, variable.second);

            variables[variable.first] = j;
        }

        root["trigger_variables"][std::to_string(test.first)] = variables;
    }


    std::ofstream file(SETTINGS_FILE);

    file << root.dump(4);

    file.close();
}

void SettingsSystem::setTriggerVariable(AbstractTriggerTest *trigger, const std::string &name, const DataValue &value)
{
    auto hash = m_hash(trigger->name() + trigger->description());

    auto findIterator = m_triggerVariables.find(hash);
    if (findIterator == m_triggerVariables.end())
    {
        m_triggerVariables[hash] = std::map<std::string, DataValue>();
        m_triggerVariables[hash][name] = value;
    }
    else
    {
        findIterator->second[name] = value;
    }

    saveData();
}

DataValue SettingsSystem::getTriggerVariable(AbstractTriggerTest *trigger, const std::string &name, bool *ok)
{
    auto hash = m_hash(trigger->name() + trigger->description());

    if (m_triggerVariables.find(hash) == m_triggerVariables.end())
    {
        if (ok)
        {
            *ok = false;
        }
        return DataValue();
    }

    if (m_triggerVariables[hash].find(name) == m_triggerVariables[hash].end())
    {
        if (ok)
        {
            *ok = false;
        }
        return DataValue();
    }

    if (ok)
    {
        *ok = true;
    }
    return m_triggerVariables[hash][name];
}
