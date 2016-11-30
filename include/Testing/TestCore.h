//
// Created by megaxela on 21.11.16.
//

#ifndef FRTESTER_TESTCORE_H
#define FRTESTER_TESTCORE_H

#include <vector>
#include <string>
#include "AbstractTriggerTest.h"
#include "AbstractTest.h"

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
     * @brief Метод для запуска тестов.
     * Тестирование происходит в одном потоке.
     */
    void runTests();

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

private:

    /**
     * @brief Приватный конструктор.
     */
    TestCore();

    /**
     * @brief Метод для восстановления состояния
     * ФР. Начальное состояние ФР:
     * 4 режим.
     */
    bool restoreFRState();

    std::vector<TriggerTestPtr> m_triggers;

    std::vector<TestPtr> m_tests;

    TestDriver m_testDriver;

    bool m_triggerFailed;

    std::vector<TriggerTestPtr> m_failedTriggers;

    TestPtr m_failedTest;

    std::vector<TriggerTestPtr> m_allFailedTriggers;
};


#endif //FRTESTER_TESTCORE_H
