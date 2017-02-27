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
 *     },
 *     "trigger_variables" : {
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
    static const std::string GlobalTabSelected;
    static const std::string TestsSharedTestsPath;
    static const std::string TestsSharedTriggersPath;
    static const std::string TestsManualTestsPath;

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
    std::string getValue(const std::string& name, const std::string& defaultValue=std::string());

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
    void setTestVariable(AbstractTest* test, const std::string& name, const DataValue &value);

    /**
     * @brief Метод для получения значения переменной теста.
     * @param test Объект теста.
     * @param name Название переменной.
     * @return Значение.
     */
    DataValue getTestVariable(AbstractTest* test, const std::string& name, bool* ok=nullptr);

    /**
     * @brief Метод для задачи значения переменной триггера.
     * @param trigger Объект триггера.
     * @param name Название переменной.
     * @param value Значение.
     */
    void setTriggerVariable(AbstractTriggerTest* trigger, const std::string& name, const DataValue& value);

    /**
     * @brief Метод для получения значения переменной триггера.
     * @param trigger Объект триггера.
     * @param name Название переменной.
     * @param ok Успешность получения значений.
     * @return Значение.
     */
    DataValue getTriggerVariable(AbstractTriggerTest* trigger, const std::string& name, bool* ok=nullptr);

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
    std::map<size_t, std::map<std::string, DataValue>> m_testVariables;
    std::map<size_t, std::map<std::string, DataValue>> m_triggerVariables;
    std::hash<std::string> m_hash;
};


#endif //FRTESTER_SETTINGSSYSTEM_H
