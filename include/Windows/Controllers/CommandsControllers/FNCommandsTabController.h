//
// Created by megaxela on 13.12.16.
//

#ifndef FRTESTER_FNCOMMANDSCONTROLLER_H
#define FRTESTER_FNCOMMANDSCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>

/**
 * @brief Класс, контроллирующий вкладку ФН в виджете с командами.
 */
class FNCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский объект.
     */
    FNCommandsTabController(Ui::MainWindow *ptr, QWidget *parent, QTabWidget *tabWidget);

protected:
    void setupConnections() override;

    void configureWidgets() override;
};


#endif //FRTESTER_FNCOMMANDSCONTROLLER_H
