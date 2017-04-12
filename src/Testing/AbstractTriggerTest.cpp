//
// Created by megaxela on 22.11.16.
//

#include <Tools/Logger.h>
#include <Testing/TestEnvironment.h>
#include <Testing/SettingsSystem.h>
#include "Testing/AbstractTriggerTest.h"

AbstractTriggerTest::AbstractTriggerTest(const std::string &name,
                                         const std::string &description,
                                         const std::vector<std::string> &group,
                                         bool critical,
                                         const std::vector<std::pair<std::string, DataValue>> &fields) :
    m_name(name),
    m_description(description),
    m_critical(critical),
    m_environment(nullptr),
    m_dynamicValues(fields),
    m_group(group)
{
    for (auto& value : m_dynamicValues)
    {
        bool ok = false;

        auto loaded = SettingsSystem::instance().getTriggerVariable(
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

AbstractTriggerTest::~AbstractTriggerTest()
{

}

std::string AbstractTriggerTest::name() const
{
    return m_name;
}

std::string AbstractTriggerTest::description() const
{
    return m_description;
}

void AbstractTriggerTest::setName(const std::string &name)
{
    m_name = name;
}

void AbstractTriggerTest::setDescription(const std::string &description)
{
    m_description = description;
}

bool AbstractTriggerTest::isCritical() const
{
    return m_critical;
}

TestEnvironment *AbstractTriggerTest::environment() const
{
    return m_environment;
}

std::vector<std::pair<std::string, DataValue::Type>> AbstractTriggerTest::getVariables() const
{
    std::vector<std::pair<std::string, DataValue::Type>> result;

    for (auto& data : m_dynamicValues)
    {
        result.push_back(std::make_pair(data.first, data.second.type));
    }

    return result;
}

void AbstractTriggerTest::setValue(const std::string &name, const DataValue &value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == value.type)
        {
            element.second = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }
    throw std::length_error("Test has no value with name \"" + name + "\".");
}


DataValue::Type AbstractTriggerTest::getValueType(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name)
        {
            return value.second.type;
        }
    }

    throw std::length_error("Test has no value with name \"" + name + "\".");
}

bool AbstractTriggerTest::containsValue(const std::string &name) const
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

bool AbstractTriggerTest::containsValue(const std::string &name, DataValue::Type type) const
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

void AbstractTriggerTest::setEnvironment(TestEnvironment *environment)
{
    m_environment = environment;
}

DataValue AbstractTriggerTest::getValue(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name)
        {
            return value.second;
        }
    }

    throw std::length_error("Trigger has no value with name \"" + name + "\".");
}

std::vector<std::string> AbstractTriggerTest::group() const
{
    return std::vector<std::string>();
}
