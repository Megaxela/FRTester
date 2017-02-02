//
// Created by megaxela on 22.11.16.
//

#include <include/Testing/TestEnvironment.h>
#include <include/Testing/AbstractTest.h>
#include <include/Testing/SettingsSystem.h>
#include "include/Testing/AbstractTest.h"

AbstractTest::AbstractTest(TestEnvironment *driver,
                           const std::string &name,
                           const std::string &description,
                           const std::vector<std::pair<std::string, DataValue>> &fields) :
    m_enviroment(driver),
    m_name(name),
    m_description(description),
    m_dynamicValues(fields)
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

TestEnvironment *AbstractTest::enviroment()
{
    return m_enviroment;
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

std::vector<std::pair<std::string, AbstractTest::DataValue::Type>> AbstractTest::getVariables() const
{
    std::vector<std::pair<std::string, AbstractTest::DataValue::Type>> result;

    for (auto& data : m_dynamicValues)
    {
        result.push_back(std::make_pair(data.first, data.second.type));
    }

    return result;
}

void AbstractTest::setValue(const std::string &name, uint8_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt8)
        {
            element.second.value.integer.uint8 = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint8 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, int8_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int8)
        {
            element.second.value.integer.int8 = value;

            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int8 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, uint16_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt16)
        {
            element.second.value.integer.uint16 = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint16 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, int16_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int16)
        {
            element.second.value.integer.int16 = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int16 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, uint32_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt32)
        {
            element.second.value.integer.uint32 = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint32 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, int32_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int32)
        {
            element.second.value.integer.int32 = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int32 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, uint64_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::UInt64)
        {
            element.second.value.integer.uint64 = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no uint64 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, int64_t value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Int64)
        {
            element.second.value.integer.int64 = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no int64 value with name \"" + name + "\".");
}

void AbstractTest::setValue(const std::string &name, const AbstractTest::DataValue &value)
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

void AbstractTest::setValue(const std::string &name, bool value)
{
    for (auto& element : m_dynamicValues)
    {
        if (element.first == name && element.second.type == DataValue::Type::Boolean)
        {
            element.second.value.boolean = value;
            SettingsSystem::instance().setTestVariable(
                    this,
                    element.first,
                    element.second
            );
            return;
        }
    }

    throw std::length_error("Test has no boolean value with name \"" + name + "\".");
}

uint8_t AbstractTest::getValueUInt8(const std::string &name) const
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

int8_t AbstractTest::getValueInt8(const std::string &name) const
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

uint16_t AbstractTest::getValueUInt16(const std::string &name) const
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

int16_t AbstractTest::getValueInt16(const std::string &name) const
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

uint32_t AbstractTest::getValueUInt32(const std::string &name) const
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

int32_t AbstractTest::getValueInt32(const std::string &name) const
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

uint64_t AbstractTest::getValueUInt64(const std::string &name) const
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

int64_t AbstractTest::getValueInt64(const std::string &name) const
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

bool AbstractTest::getValueBoolean(const std::string &name) const
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

AbstractTest::DataValue::Type AbstractTest::getValueType(const std::string &name) const
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

bool AbstractTest::containsValue(const std::string &name, AbstractTest::DataValue::Type type) const
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
