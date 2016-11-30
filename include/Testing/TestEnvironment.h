//
// Created by megaxela on 28.11.16.
//

#ifndef FRTESTER_TESTENVIROMENT_H
#define FRTESTER_TESTENVIROMENT_H


#include "TestDriver.h"

/**
 * @brief Класс, описывающий тестовое окружение.
 * Весь инструментарий, доступный для тестирования
 * будет храниться здесь.
 */
class TestEnviroment
{
public:
    TestEnviroment(TestDriver* driver);

    TestDriver* driver() const;

private:
    TestDriver* m_driver;
};


#endif //FRTESTER_TESTENVIROMENT_H
