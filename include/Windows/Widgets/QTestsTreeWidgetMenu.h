//
// Created by megaxela on 01.12.16.
//

#ifndef FRTESTER_QTESTTREEWIDGETTESTSMENU_H
#define FRTESTER_QTESTTREEWIDGETTESTSMENU_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>

/**
 * @brief Класс, описывающий контекстное меню,
 * выпадающее в QTestsTreeWidget при нажатии
 * правой кнопкой мыши на тесте или группе тестов.
 */
class QTestsTreeWidgetMenu : public QMenu
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     * @param parent Родительский объект. В данном
     * случае QTestsTreeWidget объект.
     */
    QTestsTreeWidgetMenu(QWidget* parent);

    /**
     * @brief Метод для установки режима для
     * нескольких объектов или для одного.
     * @param state Режим.
     */
    void setMultipleMode(bool state);

signals:
    /**
     * @brief Сигнал, вызываемый при изменении
     * со стороны меню состояния элементов.
     * @param enabled
     */
    void selectionEnabledChanged(bool enabled);

    /**
     * @brief Сигнал, вызываемый при запуске
     * конкретного теста.
     */
    void selectionExecuted();

private:
    bool m_multipleMode;
};


#endif //FRTESTER_QTESTTREEWIDGETTESTSMENU_H
