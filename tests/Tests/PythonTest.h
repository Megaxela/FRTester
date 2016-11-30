//
// Created by megaxela on 28.11.16.
//

#ifndef FRTESTER_PYTHONTEST_H
#define FRTESTER_PYTHONTEST_H

#include <Testing/AbstractTest.h>

/**
 * @brief Тест, обрабатывающий любой
 * python тест. Фактически - обертка
 * вокруг python теста.
 */
class PythonTest : public AbstractTest
{
public:
    /**
     * @brief Конструктор.
     */
    PythonTest(TestEnvironment* environment);

    /**
     * @brief Деструктор.
     */
    ~PythonTest();

    /**
     * @brief Метод для загрузки модуля в тест.
     * @param moduleName Название модуля.
     * @return Результат загрузки модуля.
     */
    bool loadModule(const std::string &moduleName);

    /**
     * @brief Метод для статической загрузки теста.
     * @param moduleName
     * @return
     */
    static TestPtr loadTest(const std::string &moduleName);

    /**
     * @brief Метод - исполнитель
     * @return Успешность выполнения теста.
     */
    bool execute() override;

private:

};


#endif //FRTESTER_PYTHONTEST_H
