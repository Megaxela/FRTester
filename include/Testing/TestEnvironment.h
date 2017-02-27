//
// Created by megaxela on 28.11.16.
//

#ifndef FRTESTER_TESTENVIROMENT_H
#define FRTESTER_TESTENVIROMENT_H


#include "TestDriver.h"
#include "TestingTools.h"
/**
 * @brief Класс, описывающий тестовое окружение.
 * Весь инструментарий, доступный для тестирования
 * будет храниться здесь.
 */
class TestEnvironment
{
public:
    /**
     * @brief Конструктор.
     * @param driver Тестовый драйвер.
     */
    TestEnvironment(
            TestDriver* driver,
            TestLogger* logger,
            TestingTools* tools
    );

    /**
     * @brief Метод для получения тестового драйвера.
     * @return Тестовый драйвер.
     */
    TestDriver* driver() const;

    /**
     * @brief Метод для получения логгера.
     * @return Тестовый логгер.
     */
    TestLogger* logger() const;

    /**
     * @brief Метод для полуения дополнительного инструментария.
     * @return Объект тестового инструментария.
     */
    TestingTools* tools() const;

private:
    TestDriver* m_driver;
    TestLogger* m_logger;
    TestingTools* m_tools;
};


#endif //FRTESTER_TESTENVIROMENT_H
