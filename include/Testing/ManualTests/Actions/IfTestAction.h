//
// Created by megaxela on 22.02.17.
//

#ifndef FRTESTER_IFTESTACTION_H
#define FRTESTER_IFTESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Метод для выполнения определенной последовательности
 * действий при выполнении определенного условия.
 */
class IfTestAction : public AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    IfTestAction();

    /**
     * @brief Деструктор.
     */
    ~IfTestAction();

    bool execute() override;

    virtual std::string getFullName() const;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_IFTESTACTION_H
