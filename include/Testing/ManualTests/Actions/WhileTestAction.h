//
// Created by megaxela on 21.02.17.
//

#ifndef FRTESTER_WHILETESTACTION_H
#define FRTESTER_WHILETESTACTION_H


#include <include/Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Класс, описывающий цикл while.
 */
class WhileTestAction : public AbstractTestAction
{
public:
    /**
     * @brief Конструктор.
     */
    WhileTestAction();

    /**
     * @brief Деструктор.
     */
    ~WhileTestAction();

    bool execute() override;

    std::string getFullName() const override;

    void populateTreeWidgetItem(QTreeWidgetItem *item) const override;

protected:
    TestActionPtr createAction() const override;
};


#endif //FRTESTER_WHILETESTACTION_H
