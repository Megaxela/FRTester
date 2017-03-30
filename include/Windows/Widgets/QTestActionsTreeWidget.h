//
// Created by megaxela on 17.02.17.
//

#ifndef FRTESTER_QTESTACTIONSTREEWIDGET_H
#define FRTESTER_QTESTACTIONSTREEWIDGET_H

#include <QTreeWidget>
#include <QVector>
#include <Testing/ManualTests/AbstractTestAction.h>

/**
 * @brief Класс, описывающий виджет для
 * оперирования с доступными действиями для
 * мануальных тестов.
 */
class QTestActionsTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     */
    QTestActionsTreeWidget(QWidget* parent);

    /**
     * @brief Деструктор.
     */
    virtual ~QTestActionsTreeWidget();

    /**
     * @brief Метод для передачи фильтра названий.
     * @param string Фильтр названий.
     */
    void setSearchFilter(const QString &string);

signals:

    /**
     * @brief Сигнал, вызываемый при выборе действия
     * в поле.
     * @param actionPtr Указатель на действие, находящееся
     * в treeWidget.
     */
    void onActionSelected(TestActionPtr actionPtr);

protected:
    QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const override;

private slots:

    /**
     * @brief Слот, вызываемый при смене выбранного элемента
     * виджета.
     * @param item Выбранный элемент.
     */
    void onElementSelected(QTreeWidgetItem* item);

private:

    /**
     * @brief Метод для отображения действий,
     * удовлетворяющих фильтру названий.
     */
    void displayActions();

    struct Action
    {
        Action() :
            item(nullptr),
            categories(),
            action()
        {}

        Action(QVector<QString> categories, TestActionPtr action) :
            item(nullptr),
            categories(categories),
            action(action)
        {}

        Action(const Action& action) :
            item(action.item),
            categories(action.categories),
            action(action.action)
        {}

        Action& operator=(const Action& action)
        {
            this->item = action.item;
            this->categories = action.categories;
            this->action = action.action;

            return *this;
        }

        QTreeWidgetItem* item;
        QVector<QString> categories;
        TestActionPtr action;
    };

    QRegExp m_searchFilter;
    QVector<Action> m_action;
};


#endif //FRTESTER_QTESTACTIONSTREEWIDGET_H
