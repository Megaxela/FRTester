//
// Created by megaxela on 03.02.17.
//

#ifndef FRTESTER_TESTDATAVALUE_H
#define FRTESTER_TESTDATAVALUE_H

#include <cstdint>

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
        Boolean
    };

    DataValue() :
            type(Type::UInt32)
    {
        value.boolean = false;
        value.integer.uint64 = 0;
    }

    DataValue(bool val) :
            type(Type::Boolean)
    {
        value.boolean = val;
    }

    DataValue(uint8_t number) :
            type(Type::UInt8)
    {
        value.integer.uint8 = number;
    }

    DataValue(int8_t number) :
            type(Type::Int8)
    {
        value.integer.int8 = number;
    }

    DataValue(uint16_t number) :
            type(Type::UInt16)
    {
        value.integer.uint16 = number;
    }

    DataValue(int16_t number) :
            type(Type::Int16)
    {
        value.integer.int16 = number;
    }

    DataValue(uint32_t number) :
            type(Type::UInt32)
    {
        value.integer.uint32 = number;
    }

    DataValue(int32_t number) :
            type(Type::Int32)
    {
        value.integer.int32 = number;
    }

    DataValue(uint64_t number) :
            type(Type::UInt64)
    {
        value.integer.uint64 = number;
    }

    DataValue(int64_t number) :
            type(Type::Int64)
    {
        value.integer.int64 = number;
    }

    Type type;

    struct {
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
    } value;
};


#endif //FRTESTER_TESTDATAVALUE_H
