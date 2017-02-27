//
// Created by megaxela on 22.02.17.
//

#ifndef FRTESTER_EXITIFTESTACTION_H
#define FRTESTER_EXITIFTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Класс, описывающий действие выхода из теста,
 * при выполнении какого-либо действия
 */
class ExitIfTestAction : public AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    ExitIfTestAction();

    /**
     * @brief Деструктор.
     */
    ~ExitIfTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_EXITIFTESTACTION_H
