//
// Created by megaxela on 03.02.17.
//

#ifndef FRTESTER_TESTDATAVALUE_H
#define FRTESTER_TESTDATAVALUE_H

#include <cstdint>
#include <stdexcept>
#include <libraries/json.hpp>
#include <Tools/ByteArray.h>

using nlohmann::json;

struct DataValue
{
    enum class Type
    {
        UInt8,
        Int8,
        UInt16,
        Int16,
        UInt32,
        Int32,
        UInt64,
        Int64,
        Boolean,
        String,
        ByteArray
    };

    DataValue() :
            type(Type::UInt32),
            value()

    {
        value.boolean = false;
        value.integer.uint64 = 0;
    }

    DataValue(bool val) :
            type(Type::Boolean),
            value()
    {
        value.boolean = val;
    }

    DataValue(uint8_t number) :
            type(Type::UInt8),
            value()
    {
        value.integer.uint8 = number;
    }

    DataValue(int8_t number) :
            type(Type::Int8),
            value()
    {
        value.integer.int8 = number;
    }

    DataValue(uint16_t number) :
            type(Type::UInt16),
            value()
    {
        value.integer.uint16 = number;
    }

    DataValue(int16_t number) :
            type(Type::Int16),
            value()
    {
        value.integer.int16 = number;
    }

    DataValue(uint32_t number) :
            type(Type::UInt32),
            value()
    {
        value.integer.uint32 = number;
    }

    DataValue(int32_t number) :
            type(Type::Int32),
            value()
    {
        value.integer.int32 = number;
    }

    DataValue(uint64_t number) :
            type(Type::UInt64),
            value()
    {
        value.integer.uint64 = number;
    }

    DataValue(int64_t number) :
            type(Type::Int64),
            value()
    {
        value.integer.int64 = number;
    }

    DataValue(const std::string& str) :
            type(Type::String),
            value()
    {
        value.string = str;
    }

    DataValue(const ByteArray& byteArray) :
            type(Type::ByteArray),
            value()
    {
        value.byteArray = byteArray;
    }

    uint64_t toUInt64() const
    {
        if (type != Type::UInt64)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.uint64;
    }

    int64_t toInt64() const
    {
        if (type != Type::Int64)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.int64;
    }

    uint32_t toUInt32() const
    {
        if (type != Type::UInt32)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.uint32;
    }

    int32_t toInt32() const
    {
        if (type != Type::Int32)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.int32;
    }

    uint16_t toUInt16() const
    {
        if (type != Type::UInt16)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.uint16;
    }

    int16_t toInt16() const
    {
        if (type != Type::Int16)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.int16;
    }

    uint8_t toUInt8() const
    {
        if (type != Type::UInt8)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.uint8;
    }

    int8_t toInt8() const
    {
        if (type != Type::Int8)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.integer.int8;
    }

    bool toBoolean() const
    {
        if (type != Type::Boolean)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.boolean;
    }

    ByteArray toByteArray() const
    {
        if (type != Type::ByteArray)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.byteArray;
    }

    std::string toString() const
    {
        if (type != Type::String)
        {
            throw std::logic_error("Неверный тип переменной.");
        }

        return value.string;
    }

    static void from_json(const json& j, DataValue& value);

    static void to_json(json& j, const DataValue& value);

    Type type;

    struct Data {
        Data() :
            integer(),
            boolean(false),
            string(),
            byteArray()
        {

        }

        union {
            uint8_t uint8;
            int8_t int8;
            uint16_t uint16;
            int16_t int16;
            uint32_t uint32;
            int32_t int32;
            uint64_t uint64;
            int64_t int64;
        } integer;
        bool boolean;
        std::string string;
        ByteArray byteArray;
    } value;
};

#endif //FRTESTER_TESTDATAVALUE_H
