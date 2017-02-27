//
// Created by megaxela on 22.02.17.
//

#ifndef FRTESTER_SENDRAWTESTACTION_H
#define FRTESTER_SENDRAWTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Класс, описывающий отправку чистых байт.
 */
class SendRawTestAction : public AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    SendRawTestAction();

    /**
     * @brief Деструктор.
     */
    ~SendRawTestAction();

    bool execute() override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_SENDRAWTESTACTION_H
