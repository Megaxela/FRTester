//
// Created by megaxela on 20.03.17.
//

#ifndef FRTESTER_BUG0017515TEST_H
#define FRTESTER_BUG0017515TEST_H

#include <include/Testing/AbstractTest.h>

/**
 * @brief В отчетах за смену с (без) гашением счетчик фискальных документов за смену (ФД ЗА СМЕНУ) уже учтены ФД открытия и закрытия смены, даже если смена не открыта или еще не закрыта.
 * Например:
 * Смена закрыта, бью отчет без гашения и вижу, что:
 * чеков за смену ФД: 0
 * ФД за смену: 2
 */
class Bug0017515Test : public AbstractTest
{
public:
    /**
     * @brief Конструктор.
     */
    Bug0017515Test();

    bool execute() override;
};


#endif //FRTESTER_BUG0017515TEST_H
