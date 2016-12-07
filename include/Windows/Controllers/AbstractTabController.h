//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_ABSTRACTTABCONTROLLER_H
#define FRTESTER_ABSTRACTTABCONTROLLER_H

#include <QtCore/QObject>

/**
 * @brief Класс, контроллирующий вкладку
 * меню.
 * @todo Перенести на этот класс всю работу с активацией дочерних контроллеров.
 */
class AbstractTabController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     */
    AbstractTabController();

    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~AbstractTabController();

    /**
     * @brief Метод, вызываемый при выборе вкладки, контроллируемой
     * реализацией контроллера.
     */
    virtual void tabSelected();

    /**
     * @brief Метод, вызываемый при покидании вкладки, контроллируемой
     * реализации контроллера.
     */
    virtual void tabLeaved();

    /**
     * @brief Метод, для инициализации контроллера.
     */
    virtual void init();

protected:
    /**
     * @brief Метод для настройки соединений сигналов со слотами.
     */
    virtual void setupConnections() = 0;

    /**
     * @brief Метод для настройки виджетов.
     */
    virtual void configureWidgets() = 0;
};


#endif //FRTESTER_ABSTRACTTABCONTROLLER_H
