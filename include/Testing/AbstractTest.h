//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_ABSTRACTTEST_H
#define FRTESTER_ABSTRACTTEST_H

#include <memory>
#include "TestDriver.h"
#include "TestEnvironment.h"
#include "TestDataValue.h"

#define CHECK_TRIGGERS \
if (TestCore::instance().hasFailedTriggers()) \
{ \
    environment()->logger()->log("Найдены проваленные триггеры."); \
    return false; \
}

#define CHECK_IS_TEST_RUNNING \
if (environment()->tools()->testingStoped())\
{\
    environment()->logger()->log("Тестирование остановлено");\
    return true;\
}\

class AbstractTest;

typedef std::shared_ptr<AbstractTest> TestPtr;

/**
 * @brief Класс, описывающий тест.
 */
class AbstractTest
{
public:

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
     * @param name Название переменной.
     * @return Значение.
     */
    DataValue getValue(const std::string& name);

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

    /**
     * @brief Метод для получения тестировочного окружения.
     * @return
     */
    TestEnvironment* environment();

    /**
     * @brief Метод для установки тестировочного окружения.
     * @param environment Тестировочное окружение.
     */
    void setEnvironment(TestEnvironment* environment);

private:
    TestEnvironment* m_environment;

    std::string m_name;
    std::string m_description;
    std::vector<std::pair<std::string, DataValue>> m_dynamicValues;

};


#endif //FRTESTER_ABSTRACTTEST_H
