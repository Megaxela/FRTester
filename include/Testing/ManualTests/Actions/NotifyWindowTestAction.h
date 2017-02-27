//
// Created by megaxela on 22.02.17.
//

#ifndef FRTESTER_MODALWINDOWTESTACTION_H
#define FRTESTER_MODALWINDOWTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

class NotifyWindowTestAction : public AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    NotifyWindowTestAction();

    /**
     * @brief Деструктор.
     */
    ~NotifyWindowTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_MODALWINDOWTESTACTION_H
