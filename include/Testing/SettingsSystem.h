//
// Created by megaxela on 02.02.17.
//

#ifndef FRTESTER_SETTINGSSYSTEM_H
#define FRTESTER_SETTINGSSYSTEM_H

#include <include/Testing/AbstractTest.h>
#include <map>

/**
 * @brief Класс-синглетон для работы с настройками приложения.
 * Пример сохраненного файла
 * {
 *     "settings" : {
 *         "connection_ip_address" : "192.168.137.111"
 *     },
 *     "test_variables" : {
 *         "hash" : {
 *             "variable_name" : {
 *                 "type" : "uint32_t",
 *                 "value": 22
 *             }
 *         }
 *     }
 * }
 */
class SettingsSystem
{
public:
    static const std::string ConnectionIPAddress;
    static const std::string ConnectionIPPort;
    static const std::string ConnectionCOMPort;
    static const std::string ConnectionCOMSpeed;

    /**
     * @brief Деструктор.
     */
    ~SettingsSystem();

    /**
     * @brief Статический метод для получения объекта класса.
     */
    static SettingsSystem& instance();

    /**
     * @brief Метод для получения значения.
     * @param name Название переменной.
     * @return Значение переменной.
     */
    std::string getValue(const std::string& name);

    /**
     * @brief Метод для задачи значения.
     * @param name Название переменной.
     * @param value Значение переменной.
     */
    void setValue(const std::string& name, const std::string& value);

    /**
     * @brief Метод для установки значения переменной теста.
     * @param test Объект теста.
     * @param name Название переменной.
     * @param value Значение.
     */
    void setTestVariable(AbstractTest* test, const std::string& name, const AbstractTest::DataValue &value);

    /**
     * @brief Метод для получения значения переменной теста.
     * @param test Объект теста.
     * @param name Название переменной.
     * @return Значение.
     */
    AbstractTest::DataValue getTestVariable(AbstractTest* test, const std::string& name, bool* ok=nullptr);

private:
    /**
     * @brief Констурктор.
     */
    SettingsSystem();

    /**
     * @brief Метод для загрузки данных из файла.
     */
    void loadData();

    /**
     * @brief Метод для сохранения данных в файл.
     */
    void saveData();

    std::map<std::string, std::string> m_values;
    std::map<size_t, std::map<std::string, AbstractTest::DataValue>> m_testVariables;
    std::hash<std::string> m_hash;
};


#endif //FRTESTER_SETTINGSSYSTEM_H
