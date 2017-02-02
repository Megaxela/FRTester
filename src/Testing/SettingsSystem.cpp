//
// Created by megaxela on 02.02.17.
//

#include <fstream>
#include <include/Tools/Logger.h>
#include <include/Testing/AbstractTest.h>
#include "include/Testing/SettingsSystem.h"
#include "libraries/json.hpp"

using json = nlohmann::json ;

const std::string SettingsSystem::ConnectionIPAddress  = "connection_ip_address";
const std::string SettingsSystem::ConnectionCOMPort = "connection_com_port";
const std::string SettingsSystem::ConnectionCOMSpeed = "connection_com_speed";
const std::string SettingsSystem::ConnectionIPPort = "connection_ip_port";

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
                                     const AbstractTest::DataValue &value)
{
    auto hash = m_hash(test->name() + test->description());

    auto findIterator = m_testVariables.find(hash);
    if (findIterator == m_testVariables.end())
    {
        m_testVariables[hash] = std::map<std::string, AbstractTest::DataValue>();
        m_testVariables[hash][name] = value;
    }
    else
    {
        findIterator->second[name] = value;
    }
}

AbstractTest::DataValue SettingsSystem::getTestVariable(AbstractTest* test,
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
        return AbstractTest::DataValue();
    }

    if (m_testVariables[hash].find(name) == m_testVariables[hash].end())
    {
        if (ok)
        {
            *ok = false;
        }
        return AbstractTest::DataValue();
    }

    if (ok)
    {
        *ok = true;
    }
    return m_testVariables[hash][name];
}

SettingsSystem::SettingsSystem()
{
    loadData();
}

std::string SettingsSystem::getValue(const std::string &name)
{
    if (m_values.find(name) == m_values.end())
    {
        return std::string();
    }

    return m_values[name];
}

void SettingsSystem::setValue(const std::string &name, const std::string &value)
{
    m_values[name] = value;
}

void SettingsSystem::loadData()
{
    // Loading file with data
    if (!SystemTools::Path::fileExists("save.json"))
    {
        Warning("Не существует файла сохранений.");
        return;
    }

    std::ifstream file("save.json");

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
    for (json::iterator test = testVariablesDict.begin(); test != testVariablesDict.end(); ++test)
    {
        auto longKey = std::stoul(test.key());
        m_testVariables[longKey] = std::map<std::string, AbstractTest::DataValue>();
        json value = test.value();
        for (json::iterator variable = value.begin(); variable  != value.end(); ++variable)
        {
            AbstractTest::DataValue value;
            std::string varType = variable.value()["type"];
            if (varType == "uint8_t")
            {
                value.type = AbstractTest::DataValue::Type::UInt8;
                value.value.integer.uint8 = variable.value()["value"];
            }
            else if (varType == "int8_t")
            {
                value.type = AbstractTest::DataValue::Type::Int8;
                value.value.integer.int8 = variable.value()["value"];
            }
            else if (varType == "uint16_t")
            {
                value.type = AbstractTest::DataValue::Type::UInt16;
                value.value.integer.uint16 = variable.value()["value"];
            }
            else if (varType == "int16_t")
            {
                value.type = AbstractTest::DataValue::Type::Int16;
                value.value.integer.int16 = variable.value()["value"];
            }
            else if (varType == "uint32_t")
            {
                value.type = AbstractTest::DataValue::Type::UInt32;
                value.value.integer.uint32 = variable.value()["value"];
            }
            else if (varType == "int32_t")
            {
                value.type = AbstractTest::DataValue::Type::Int32;
                value.value.integer.int32 = variable.value()["value"];
            }
            else if (varType == "uint64_t")
            {
                value.type = AbstractTest::DataValue::Type::UInt64;
                value.value.integer.uint64 = variable.value()["value"];
            }
            else if (varType == "int64_t")
            {
                value.type = AbstractTest::DataValue::Type::Int64;
                value.value.integer.int64 = variable.value()["value"];
            }
            else if (varType == "boolean")
            {
                value.type = AbstractTest::DataValue::Type ::Boolean;
                value.value.boolean = variable.value()["value"];
            }

            m_testVariables[longKey][variable.key()] = value;
        }
    }
}

void SettingsSystem::saveData()
{
    json root;

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
            switch (variable.second.type)
            {
            case AbstractTest::DataValue::Type::UInt8:
                variables[variable.first]["type"] = "uint8_t";
                variables[variable.first]["value"] = variable.second.value.integer.uint8;
                break;
            case AbstractTest::DataValue::Type::Int8:
                variables[variable.first]["type"] = "int8_t";
                variables[variable.first]["value"] = variable.second.value.integer.int8;
                break;
            case AbstractTest::DataValue::Type::UInt16:
                variables[variable.first]["type"] = "uint16_t";
                variables[variable.first]["value"] = variable.second.value.integer.uint16;
                break;
            case AbstractTest::DataValue::Type::Int16:
                variables[variable.first]["type"] = "int16_t";
                variables[variable.first]["value"] = variable.second.value.integer.int16;
                break;
            case AbstractTest::DataValue::Type::UInt32:
                variables[variable.first]["type"] = "uint32_t";
                variables[variable.first]["value"] = variable.second.value.integer.uint32;
                break;
            case AbstractTest::DataValue::Type::Int32:
                variables[variable.first]["type"] = "int32_t";
                variables[variable.first]["value"] = variable.second.value.integer.int32;
                break;
            case AbstractTest::DataValue::Type::UInt64:
                variables[variable.first]["type"] = "uint64_t";
                variables[variable.first]["value"] = variable.second.value.integer.uint64;
                break;
            case AbstractTest::DataValue::Type::Int64:
                variables[variable.first]["type"] = "int64_t";
                variables[variable.first]["value"] = variable.second.value.integer.int64;
                break;
            case AbstractTest::DataValue::Type::Boolean:
                variables[variable.first]["type"] = "boolean";
                variables[variable.first]["value"] = variable.second.value.boolean;
                break;
            }
        }

        root["test_variables"][std::to_string(test.first)] = variables;
    }

    std::ofstream file("save.json");

    file << root;

    file.close();
}
