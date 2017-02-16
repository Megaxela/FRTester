//
// Created by megaxela on 03.02.17.
//

#ifndef FRTESTER_TESTINGTOOLS_H
#define FRTESTER_TESTINGTOOLS_H

#include <include/FRDriver.h>
#include "TestDriver.h"

class TestLogger;
class TestingExecutor;

/**
 * @brief Класс, описывающий тестовые инструменты для
 * быстрого использования часто используемых функций.
 */
class TestingTools
{
    friend class TestCore;

public:
    enum class Printing
    {
        Unknown,
        Enabled,
        DisabledForOneCheck,
        Disabled
    };

    /**
     * @brief Конструктор.
     */
    TestingTools(TestDriver* testDriver, TestLogger* environment);

    /**
     * @brief Деструктор.
     */
    ~TestingTools();

    /**
     * @brief Метод для ожидания окончания печати.
     * @param timeout Таймаут, после которого ожидание
     * прекращается и возвращается false. Если timeout = 0
     * то таймаута нет.
     * @return Результат ожидания.
     */
    bool waitForPrintingFinished(uint32_t password, uint32_t timeout=0);

    /**
     * @brief Метод для переключения используемого
     * драйвера с тестировочного на обычный.
     * @param enabled Использовать ли тестировочный драйвер.
     */
    void enableTesting(bool enabled);

    /**
     * @brief Метод для смены режима печати.
     * @param password Пароль для чтения и записи таблиц.
     * @param mode Новый режим печати.
     * @return Старый режим печати.
     */
    Printing enablePrinting(uint32_t password, Printing mode);

    /**
     * @brief Метод для проверки работает ли еще тестирование.
     * @return Работает ли еще тестирование.
     */
    bool testingStoped();

private:
    TestingExecutor* m_executor;

    FRDriver* m_currentDriver;
    TestDriver* m_testDriver;
    TestLogger* m_logger;
};


#endif //FRTESTER_TESTINGTOOLS_H
