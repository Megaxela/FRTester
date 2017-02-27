//
// Created by megaxela on 17.02.17.
//

#ifndef FRTESTER_BEEPTESTACTION_H
#define FRTESTER_BEEPTESTACTION_H

#include <include/Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Класс, описывающий действие beep.
 */
class BeepTestAction : public AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    BeepTestAction();

    /**
     * @brief Деструктор.
     */
    ~BeepTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_BEEPTESTACTION_H
