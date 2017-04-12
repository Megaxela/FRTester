//
// Created by megaxela on 22.11.16.
//

#include <Testing/TestEnvironment.h>
#include <Testing/AbstractTest.h>
#include <Testing/SettingsSystem.h>
#include "Testing/AbstractTest.h"

AbstractTest::AbstractTest(const std::string &name,
                           const std::string &description,
                           const std::vector<std::string> &group,
                           const std::vector<std::pair<std::string, DataValue>> &fields) :
        m_environment(nullptr),
        m_name(name),
        m_description(description),
        m_dynamicValues(fields),
        m_group(group)
{
    // Загрузка сохраненных значений
    for (auto& value : m_dynamicValues)
    {
        bool ok = false;

        auto loaded = SettingsSystem::instance().getTestVariable(
                this,
                value.first,
                &ok
        );

        if (ok)
        {
            setValue(value.first, loaded);
        }
    }
}

AbstractTest::~AbstractTest()
{

}

TestEnvironment *AbstractTest::environment()
{
    return m_environment;
}

std::string AbstractTest::name() const
{
    return m_name;
}

std::string AbstractTest::description() const
{
    return m_description;
}

void AbstractTest::setName(const std::string &name)
{
    m_name = name;
}

void AbstractTest::setDescription(const std::string &description)
{
    m_description = description;
}

std::vector<std::pair<std::string, DataValue::Type>> AbstractTest::getVariables() const
{
    std::vector<std::pair<std::string, DataValue::Type>> result;

    for (auto& data : m_dynamicValues)
    {
        result.push_back(std::make_pair(data.first, data.second.type));
    }

    return result;
}

void AbstractTest::setValue(const std::string &name, const DataValue &value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == value.type)
        {
            element.second = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }
    throw std::length_error("Test has no value with name \"" + name + "\".");
}

DataValue AbstractTest::getValue(const std::string &name)
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name)
        {
            return value.second;
        }
    }

    throw std::length_error("Test has no value with name \"" + name + "\".");
}

bool AbstractTest::containsValue(const std::string &name) const
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name)
        {
            return true;
        }
    }

    return false;
}

bool AbstractTest::containsValue(const std::string &name, DataValue::Type type) const
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == type)
        {
            return true;
        }
    }

    return false;
}

void AbstractTest::setEnvironment(TestEnvironment *environment)
{
    m_environment = environment;
}

std::vector<std::string> AbstractTest::group() const
{
    return m_group;
}
