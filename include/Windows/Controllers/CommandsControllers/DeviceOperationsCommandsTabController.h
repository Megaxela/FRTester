//
// Created by megaxela on 12.01.17.
//

#ifndef FRTESTER_DEVICEOPERATIONSCOMMANDSTABCONTROLLER_H
#define FRTESTER_DEVICEOPERATIONSCOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>

class CommandsTabController;

/**
 * @brief Класс, управляющий вкладкой с действиями устройств.
 */
class DeviceOperationsCommandsTabController : public AbstractTabController
{
public:
    DeviceOperationsCommandsTabController(Ui::MainWindow* ptr, QWidget* parent);


protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * чтения регистров устройства.
     */
    void onReadButtonPressed();

    /**
     * @brief
     */
    void onCurrentReadButtonPressed();

private:
    CommandsTabController* commandsTabController();

    std::vector<std::string> m_registerNames;
};


#endif //FRTESTER_DEVICEOPERATIONSCOMMANDSTABCONTROLLER_H
