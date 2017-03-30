//
// Created by megaxela on 20.03.17.
//

#ifndef FRTESTER_BUG0017483TEST_H
#define FRTESTER_BUG0017483TEST_H


#include <Testing/AbstractTest.h>

/**
 * @brief Тест бага:
 * Если в таблице №18 "Fiscal storage" установить значение
 * поля №7 "User" длиной в 128 символов - то при значении
 * поля №12 "Rus печать реквезитов пользователя"
 * (таблица №17 "Региональные настройки") равным 1 будет
 * печататься в одну строку поле №7 и поле №8 из таблицы №18.
 */
class Bug0017483Test : public AbstractTest
{
public:
    /**
     * @brief Конструктор.
     */
    Bug0017483Test();

    bool execute() override;

};


#endif //FRTESTER_BUG0017483TEST_H
