//
// Created by megaxela on 22.02.17.
//

#ifndef FRTESTER_TIMEOUTACTION_H
#define FRTESTER_TIMEOUTACTION_H

#include <include/Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Класс, описывающий действие, выполняющее
 * таймаут.
 */
class TimeoutTestAction : public AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    TimeoutTestAction();

    /**
     * @brief Деструктор.
     */
    ~TimeoutTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_TIMEOUTACTION_H
