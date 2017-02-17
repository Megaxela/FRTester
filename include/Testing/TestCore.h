//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_TESTCORE_H
#define FRTESTER_TESTCORE_H

#include <vector>
#include <string>
#include "AbstractTriggerTest.h"
#include "AbstractTest.h"

class TestingExecutor;

/**
 * @brief Класс, обрабатывающий основные функции
 * процесса тестирования. Синглетон, поскольку
 * ядро для тестирования должно быть одно на всю программу.
 */
class TestCore
{
public:
    /**
     * @brief Деструктор.
     */
    ~TestCore();

    /**
     * @brief Метод для получения экземпляра класса.
     * @return Экземпляр класса TestCore.
     */
    static TestCore& instance();

    /**
     * @brief Метод для получения тригеров по определенному
     * тегу.
     * @param tag Тег.
     * @param v Вектор который будет заполнен триггерами.
     */
    void getTriggers(const std::string &tag, std::vector<TriggerTestPtr>& v);

    /**
     * @brief Метод для обновления базы тестов.
     */
    void updateDatabase();

    /**
     * @brief Метод для проверки наличия не
     * обработанных триггеров, вернувших неудачу.
     * @return Наличие неуспешных тригеров.
     */
    bool hasFailedTriggers();

    /**
     * @brief Метод для получения массива с триггерами
     * которые вернули ошибку во время исполнения.
     * @return Массив с триггерами.
     */
    std::vector<TriggerTestPtr> getFailedTriggers();

    /**
     * @brief Метод для очистки триггеров, вернувших ошибку.
     */
    void clearFailedTriggers();

    /**
     * @brief Метод для указания, что какой-то
     * тригер вернул ошибку во время исполнения.
     * @param trigger Тригер.
     */
    void triggerMistaken(TriggerTestPtr trigger);

    /**
     * @brief Метод для получения набора тестов.
     * @return Массив с тестами.
     */
    std::vector<TestPtr> getTests();

    /**
     * @brief Метод для получения набора триггеров.
     * @return Массив с триггерами.
     */
    std::vector<TriggerTestPtr> getTriggers();

    /**
     * @brief Метод для получения набора активных
     * тестов.
     * @return Массив с тестами.
     */
    std::vector<TestPtr> getActiveTests();

    /**
     * @brief Метод для получения набора активных
     * триггеров.
     * @return Массив с триггерами.
     */
    std::vector<TriggerTestPtr> getActiveTriggers();

    /**
     * @brief Метод для восстановления состояния
     * ФР. Начальное состояние ФР:
     * 4 режим.
     * Для восстановления используется не тестировочная
     * версия драйвера, поэтому триггеры во время
     * восстановления вызваны не будут.
     */
    bool restoreFRState();

    /**
     * @brief Метод для прерывания тестирования.
     */
    void interruptTesting();

    /**
     * @brief Метод для установки состояния конкретного теста.
     * @param test Тест.
     * @param enabled Состояние.
     */
    void setTestEnabled(TestPtr test, bool enabled);

    /**
     * @brief Метод для установки состояния конкретного триггера.
     * @param trigger Триггер.
     * @param enabled Состояние.
     */
    void setTriggerEnabled(TriggerTestPtr trigger, bool enabled);

    /**
     * @brief Метод для передачи модуля для выполнения тестов.
     */
    void setTestExecutor(TestingExecutor* );

private:

    void addTest(TestPtr test);

    void addTrigger(TriggerTestPtr trigger);

    /**
     * @brief Приватный конструктор.
     */
    TestCore();

    struct TestData
    {
        TestPtr test;
        bool enabled;
    };

    struct TriggerData
    {
        TriggerTestPtr trigger;
        bool enabled;
    };

    void init();

    void deinit();

    std::vector<TriggerData> m_triggers;

    std::vector<TestData> m_tests;

    TestEnvironment* m_environment;

    std::vector<TriggerTestPtr> m_failedTriggers;

    uint32_t m_sysAdmPassword;

    TestingExecutor* m_testingExecutor;

    std::vector<SystemTools::LibraryType> m_sharedTests;
};


#endif //FRTESTER_TESTCORE_H
