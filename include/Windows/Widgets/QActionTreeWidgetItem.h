//
// Created by megaxela on 20.02.17.
//

#ifndef FRTESTER_QACTIONTREEWIDGETITEM_H
#define FRTESTER_QACTIONTREEWIDGETITEM_H


#include <QTreeWidgetItem>
#include <include/Testing/ManualTests/AbstractTestAction.h>

class QActionTreeWidgetItem : public QTreeWidgetItem
{
public:

    enum ItemType
    {
        ActionType = 1001
    };

    /**
     * @brief Конструктор.
     * @param view
     */
    QActionTreeWidgetItem(QTreeWidget* view, TestActionPtr action);

    /**
     * @brief Конструктор.
     * @param view
     */
    QActionTreeWidgetItem(QTreeWidgetItem* item, TestActionPtr action);

    /**
     * @brief Деструктор.
     */
    virtual ~QActionTreeWidgetItem();

    /**
     * @brief Метод для получения текущего действия.
     * @return
     */
    TestActionPtr getAction() const;

private:
    TestActionPtr m_action;
};


#endif //FRTESTER_QACTIONTREEWIDGETITEM_H
