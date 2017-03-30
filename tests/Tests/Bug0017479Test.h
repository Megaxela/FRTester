//
// Created by megaxela on 20.03.17.
//

#ifndef FRTESTER_BUG0017479TEST_H
#define FRTESTER_BUG0017479TEST_H

#include <Testing/AbstractTest.h>

/**
 * @brief Тест для проверки:
 * Если в таблице "Региональные настройки" включить печать
 * отмененных документов и в поле "Не печатать документ"
 * установить значение = 1 (если значение = 2, то все нормально),
 * то при аннулировании чека срабатывает промотка и отрезка чека,
 * а также печать заголовка для следующего документа
 */
class Bug0017479Test : public AbstractTest
{
public:
    /**
     * @brief Constructor.
     */
    Bug0017479Test();

    bool execute() override;
};


#endif //FRTESTER_BUG0017479TEST_H
