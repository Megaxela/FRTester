//
// Created by megaxela on 20.02.17.
//

#ifndef FRTESTER_QTESTDISPLAYTREEWIDGET_H
#define FRTESTER_QTESTDISPLAYTREEWIDGET_H

#include <QTreeWidget>
#include <Testing/ManualTests/ManualTest.h>

class QActionTreeWidgetItem;

/**
 * @brief Класс, описывающий виджет для
 * отображения и оперирования с командами
 * ручного теста.
 */
class QTestDisplayTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     */
    QTestDisplayTreeWidget(QWidget* parent);

    /**
     * @brief Деструктор.
     */
    virtual ~QTestDisplayTreeWidget();

    /**
     * @brief Метод для передачи теста с которым требуется работать.
     * @param test Умный указатель на тест.
     */
    void setManualTest(ManualTestPtr test);

    /**
     * @brief Метод для получения теста с которым сейчас ведется
     * работа.
     * @return Умный указатель на тест.
     */
    ManualTestPtr manualTest() const;

    /**
     * @brief Метод для передачи фильтра названий.
     * @param string Фильтр названий.
     */
    void setSearchFilter(QString value);

    /**
     * @brief Метод для перерисовки действий.
     */
    void displayActions();

    /**
     * @brief Метод для принятия изменений в тест.
     */
    void applyChanges();

public slots:

    void onActionChanged(TestActionPtr action);

signals:

    void onActionSelected(TestActionPtr action);

protected:
    void dropEvent(QDropEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    /**
     * @brief Слот, вызываемый при выборе элемента в виджете.
     * @param item Выбранный элемент.
     */
    void onElementSelected(QTreeWidgetItem* item);

private:
    void applyItem(QActionTreeWidgetItem* item, bool root = true);

    ManualTestPtr m_test;
    QRegExp m_searchFilter;
};


#endif //FRTESTER_QTESTDISPLAYTREEWIDGET_H
