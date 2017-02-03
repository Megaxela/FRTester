//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_TRIGGERTEST_H
#define FRTESTER_TRIGGERTEST_H

#include <memory>
#include "Tools/ByteArray.h"
#include "TestDataValue.h"

class TestEnvironment;
class AbstractTriggerTest;

typedef std::shared_ptr<AbstractTriggerTest> TriggerTestPtr;

/**
 * @brief Класс, оборачивающий триггер тест.
 */
class AbstractTriggerTest
{
public:
    /**
     * @brief Конструктор.
     */
    AbstractTriggerTest(TestEnvironment *environment,
                        const std::string &name,
                        const std::string &description,
                        bool critical,
                        const std::vector<std::pair<std::string, DataValue>> &fields);

    /**
     * @brief Деструктор.
     */
    virtual ~AbstractTriggerTest();

    /**
     * @brief Метод, вызываемый перед выполнением
     * команды на который данный триггер повешен.
     */
    virtual void onPreExecute(const std::string &realTag, const ByteArray &arguments) = 0;

    /**
     * @brief Метод, вызываемый после выполнения
     * команды на который данный триггер повешен.
     */
    virtual void onPostExecute() = 0;

    /**
     * @brief Метод для получения результата тестирования.
     * @return true при успешном тесте и false при неуспешном
     */
    virtual bool succeed() = 0;

    /**
     * @brief Метод для проверки наличия тега среди
     * тегов теста.
     * @return Результат проверки наличия тега в тегах теста.
     */
    virtual bool containsTag(const std::string &tag) = 0;

    /**
     * @brief Метод для получения названия теста.
     * @return Строка с названием теста.
     */
    std::string name() const;

    /**
     * @brief Метод для получения описания теста.
     * @return Строка с описанием теста.
     */
    std::string description() const;

    /**
     * @brief Метод для получения критичности триггера.
     * Если тригер критичен - то тесты будут остановлены
     * после того как этот тригер найдет ошибку. Если
     * тригер не критичен - то тесты не будут остановлены
     * но факт того, что была найдена ошибка будет записана.
     * @return Критичность триггера.
     */
    bool isCritical() const;

    /**
     * @brief Метод для установки названия теста.
     * @param name Новое название теста.
     */
    void setName(const std::string& name);

    /**
     * @brief Метод для установки описания теста.
     * @param description Новое описание теста.
     */
    void setDescription(const std::string& description);

    /**
     * @brief Метод дляп получения тестировочного
     * окружения.
     * @return Тестировочное окружение.
     */
    TestEnvironment* environment() const;

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

private:
    std::string m_name;

    std::string m_description;

    bool m_critical;

    TestEnvironment* m_environment;

    std::vector<std::pair<std::string, DataValue>> m_dynamicValues;
};


#endif //FRTESTER_TRIGGERTEST_H
