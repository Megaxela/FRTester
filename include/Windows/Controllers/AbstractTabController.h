//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_ABSTRACTTABCONTROLLER_H
#define FRTESTER_ABSTRACTTABCONTROLLER_H

/**
 * @brief Класс, контроллирующий вкладку
 * меню.
 */
class AbstractTabController
{
public:
    AbstractTabController();

    virtual ~AbstractTabController();

    virtual void tabSelected() = 0;

    virtual void setUI(void* pointer) = 0;
};


#endif //FRTESTER_ABSTRACTTABCONTROLLER_H
