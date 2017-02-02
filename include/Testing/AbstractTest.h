//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_ABSTRACTTEST_H
#define FRTESTER_ABSTRACTTEST_H

#include <memory>
#include "TestDriver.h"
#include "TestEnvironment.h"

#define CHECK_TRIGGERS \
if (TestCore::instance().hasFailedTriggers()) \
{ \
    enviroment()->logger()->log("Найдены проваленные триггеры."); \
    return false; \
}

class AbstractTest;

typedef std::shared_ptr<AbstractTest> TestPtr;

/**
 * @brief Класс, описывающий тест.
 */
class AbstractTest
{
public:

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

    /**
     * @brief Конструктор.
     * @param driver Драйвер для вызова функций.
     */
    AbstractTest(TestEnvironment *environment,
                 const std::string &name,
                 const std::string &description,
                 const std::vector<std::pair<std::string, DataValue>> &fields=std::vector<std::pair<std::string,DataValue>>());

    /**
     * @brief Деструктор.
     */
    ~AbstractTest();

    /**
     * @brief Виртуальный метод, выполняющий тест.
     * Устройство находится в начальном режиме. Оно
     * всегда будет находиться в начальном режиме.
     * Если не удасться перевести измененное состояние
     * устройства в начальное то следующий тест просто
     * не будет запущен.
     * @return Результат тестирования.
     */
    virtual bool execute() = 0;

    /**
     * @brief Метод для получения названия теста.
     * @return Название теста.
     */
    std::string name() const;

    /**
     * @brief Метод для получения описания теста.
     * @return Описание теста.
     */
    std::string description() const;

    /**
     * @brief Метод для установки названия теста.
     * @param name Название теста.
     */
    void setName(const std::string& name);

    /**
     * @brief Метод для установки описания теста.
     * @param description Описание теста.
     */
    void setDescription(const std::string& description);

    /**
     * @brief Метод для получения полей теста.
     */
    std::vector<std::pair<std::string, DataValue::Type>> getVariables() const;

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, uint8_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, int8_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, uint16_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, int16_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, uint32_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, int32_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, uint64_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, int64_t value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, bool value);

    /**
     * @brief Метод для установки значения переменной.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, const DataValue& value);

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    uint8_t getValueUInt8(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    int8_t getValueInt8(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    uint16_t getValueUInt16(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    int16_t getValueInt16(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    uint32_t getValueUInt32(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    int32_t getValueInt32(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    uint64_t getValueUInt64(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    int64_t getValueInt64(const std::string& name) const;

    /**
     * @brief Метод для получения значения переменной.
     * @param name Название переменной
     * @return Значение.
     */
    bool getValueBoolean(const std::string& name) const;

    /**
     * @brief Метод для получения типа переменной.
     * @param name Название переменной.
     * @return Тип переменной.
     */
    DataValue::Type getValueType(const std::string& name) const;

    /**
     * @brief Метод для проверки наличия динамической переменной.
     * @param name Название переменной.
     * @return Наличие переменной.
     */
    bool containsValue(const std::string& name) const;

    /**
     * @brief Метод для проверки наличия динамической переменной определенного типа.
     * @param name Название переменной.
     * @param type Тип переменной.
     * @return Наличие переменной.
     */
    bool containsValue(const std::string& name, DataValue::Type type) const;

protected:
    /**
     * @brief Метод для получения драйвера.
     * @return
     */
    TestEnvironment* enviroment();

private:
    TestEnvironment* m_enviroment;

    std::string m_name;
    std::string m_description;
    std::vector<std::pair<std::string, DataValue>> m_dynamicValues;

};


#endif //FRTESTER_ABSTRACTTEST_H
