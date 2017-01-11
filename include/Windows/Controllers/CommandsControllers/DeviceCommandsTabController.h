//
// Created by megaxela on 10.01.17.
//

#ifndef FRTESTER_DEVICECOMMANDSTABCONTROLLER_H
#define FRTESTER_DEVICECOMMANDSTABCONTROLLER_H

#include <include/Windows/Controllers/AbstractTabController.h>

/**
 * @brief Класс, контроллирующий вкладку "Устройство" в виджете
 * с командами.
 */
class DeviceCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский объект.
     * @param table Указатель на подконтрольный Tab Widget.
     */
    DeviceCommandsTabController(Ui::MainWindow* ptr, QWidget* parent, QTabWidget* table);

protected:
    void setupConnections() override;

    void configureWidgets() override;
};


#endif //FRTESTER_DEVICECOMMANDSTABCONTROLLER_H
