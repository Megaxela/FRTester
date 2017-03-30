//
// Created by megaxela on 22.11.16.
//

#include <Tools/Logger.h>
#include <Testing/TestEnvironment.h>
#include <Testing/SettingsSystem.h>
#include "Testing/AbstractTriggerTest.h"

AbstractTriggerTest::AbstractTriggerTest(TestEnvironment *environment, const std::string &name,
                                         const std::string &description, bool critical,
                                         const std::vector<std::pair<std::string, DataValue>> &fields) :
    m_name(name),
    m_description(description),
    m_critical(critical),
    m_environment(environment),
    m_dynamicValues(fields)
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

void AbstractTriggerTest::setValue(const std::string &name, uint8_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt8)
        {
            element.second.value.integer.uint8 = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint8 value with name \"" + name + "\".");
}

void AbstractTriggerTest::setValue(const std::string &name, int8_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int8)
        {
            element.second.value.integer.int8 = value;

            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int8 value with name \"" + name + "\".");
}

void AbstractTriggerTest::setValue(const std::string &name, uint16_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt16)
        {
            element.second.value.integer.uint16 = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint16 value with name \"" + name + "\".");
}

void AbstractTriggerTest::setValue(const std::string &name, int16_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int16)
        {
            element.second.value.integer.int16 = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int16 value with name \"" + name + "\".");
}

void AbstractTriggerTest::setValue(const std::string &name, uint32_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt32)
        {
            element.second.value.integer.uint32 = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint32 value with name \"" + name + "\".");
}

void AbstractTriggerTest::setValue(const std::string &name, int32_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int32)
        {
            element.second.value.integer.int32 = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int32 value with name \"" + name + "\".");
}

void AbstractTriggerTest::setValue(const std::string &name, uint64_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt64)
        {
            element.second.value.integer.uint64 = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint64 value with name \"" + name + "\".");
}

void AbstractTriggerTest::setValue(const std::string &name, int64_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int64)
        {
            element.second.value.integer.int64 = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int64 value with name \"" + name + "\".");
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

void AbstractTriggerTest::setValue(const std::string &name, bool value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Boolean)
        {
            element.second.value.boolean = value;
            SettingsSystem::instance().setTriggerVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no boolean value with name \"" + name + "\".");
}

uint8_t AbstractTriggerTest::getValueUInt8(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::UInt8)
        {
            return value.second.value.integer.uint8;
        }
    }

    throw std::length_error("Test has no uint8 value with name \"" + name + "\".");
}

int8_t AbstractTriggerTest::getValueInt8(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::Int8)
        {
            return value.second.value.integer.int8;
        }
    }

    throw std::length_error("Test has no int8 value with name \"" + name + "\".");
}

uint16_t AbstractTriggerTest::getValueUInt16(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::UInt16)
        {
            return value.second.value.integer.uint16;
        }
    }

    throw std::length_error("Test has no uint16 value with name \"" + name + "\".");
}

int16_t AbstractTriggerTest::getValueInt16(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::Int16)
        {
            return value.second.value.integer.int16;
        }
    }

    throw std::length_error("Test has no int16 value with name \"" + name + "\".");
}

uint32_t AbstractTriggerTest::getValueUInt32(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::UInt32)
        {
            return value.second.value.integer.uint32;
        }
    }

    throw std::length_error("Test has no uint32 value with name \"" + name + "\".");
}

int32_t AbstractTriggerTest::getValueInt32(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::Int32)
        {
            return value.second.value.integer.int32;
        }
    }

    throw std::length_error("Test has no int32 value with name \"" + name + "\".");
}

uint64_t AbstractTriggerTest::getValueUInt64(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::UInt64)
        {
            return value.second.value.integer.uint64;
        }
    }

    throw std::length_error("Test has no uint64 value with name \"" + name + "\".");
}

int64_t AbstractTriggerTest::getValueInt64(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::Int64)
        {
            return value.second.value.integer.int64;
        }
    }

    throw std::length_error("Test has no int64 value with name \"" + name + "\".");
}

bool AbstractTriggerTest::getValueBoolean(const std::string &name) const
{
    for (auto& value : m_dynamicValues)
    {
        if (value.first == name && value.second.type == DataValue::Type::Boolean)
        {
            return value.second.value.boolean;
        }
    }

    throw std::length_error("Test has no boolean value with name \"" + name + "\".");
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
