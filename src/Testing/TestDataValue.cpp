//
// Created by megaxela on 03.02.17.
//

#include <Testing/TestDataValue.h>
#include "Testing/TestDataValue.h"

void DataValue::from_json(const json &j, DataValue &value)
{
    std::string varType = j["type"];
    if (varType == "uint8_t")
    {
        value.type = DataValue::Type::UInt8;
        value.value.integer.uint8 = j["value"];
    }
    else if (varType == "int8_t")
    {
        value.type = DataValue::Type::Int8;
        value.value.integer.int8 = j["value"];
    }
    else if (varType == "uint16_t")
    {
        value.type = DataValue::Type::UInt16;
        value.value.integer.uint16 = j["value"];
    }
    else if (varType == "int16_t")
    {
        value.type = DataValue::Type::Int16;
        value.value.integer.int16 = j["value"];
    }
    else if (varType == "uint32_t")
    {
        value.type = DataValue::Type::UInt32;
        value.value.integer.uint32 = j["value"];
    }
    else if (varType == "int32_t")
    {
        value.type = DataValue::Type::Int32;
        value.value.integer.int32 = j["value"];
    }
    else if (varType == "uint64_t")
    {
        value.type = DataValue::Type::UInt64;
        value.value.integer.uint64 = j["value"];
    }
    else if (varType == "int64_t")
    {
        value.type = DataValue::Type::Int64;
        value.value.integer.int64 = j["value"];
    }
    else if (varType == "boolean")
    {
        value.type = DataValue::Type::Boolean;
        value.value.boolean = j["value"];
    }
    else if (varType == "string")
    {
        value.type = DataValue::Type::String;
        value.value.string = j["value"];
    }
    else if (varType == "byteArray")
    {
        value.type = DataValue::Type::ByteArray;
        value.value.byteArray = ByteArray::fromHex(j["value"]);
    }
}

void DataValue::to_json(json &j, const DataValue &value)
{
    switch (value.type)
    {
    case DataValue::Type::UInt8:
        j["type"] = "uint8_t";
        j["value"] = value.value.integer.uint8;
        break;
    case DataValue::Type::Int8:
        j["type"] = "int8_t";
        j["value"] = value.value.integer.int8;
        break;
    case DataValue::Type::UInt16:
        j["type"] = "uint16_t";
        j["value"] = value.value.integer.uint16;
        break;
    case DataValue::Type::Int16:
        j["type"] = "int16_t";
        j["value"] = value.value.integer.int16;
        break;
    case DataValue::Type::UInt32:
        j["type"] = "uint32_t";
        j["value"] = value.value.integer.uint32;
        break;
    case DataValue::Type::Int32:
        j["type"] = "int32_t";
        j["value"] = value.value.integer.int32;
        break;
    case DataValue::Type::UInt64:
        j["type"] = "uint64_t";
        j["value"] = value.value.integer.uint64;
        break;
    case DataValue::Type::Int64:
        j["type"] = "int64_t";
        j["value"] = value.value.integer.int64;
        break;
    case DataValue::Type::Boolean:
        j["type"] = "boolean";
        j["value"] = value.value.boolean;
        break;
    case DataValue::Type::String:
        j["type"] = "string";
        j["value"] = value.value.string;
        break;
    case DataValue::Type::ByteArray:
        j["type"] = "byteArray";
        j["value"] = value.value.byteArray.toHex();
        break;
    }
}
