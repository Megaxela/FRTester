//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_ABSTRACTTABCONTROLLER_H
#define FRTESTER_ABSTRACTTABCONTROLLER_H

#include <QtCore/QObject>

/**
 * @brief Класс, контроллирующий вкладку
 * меню.
 */
class AbstractTabController : public QObject
{
    Q_OBJECT

public:
    AbstractTabController();

    virtual ~AbstractTabController();

    virtual void tabSelected();

    virtual void tabLeaved();

    virtual void init();

protected:
    virtual void setupConnections() = 0;

    virtual void configureWidgets() = 0;
};


#endif //FRTESTER_ABSTRACTTABCONTROLLER_H
