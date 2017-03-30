//
// Created by megaxela on 10.01.17.
//

#ifndef FRTESTER_DEVICEMONEYCOMMANDSTABCONTROLLER_H
#define FRTESTER_DEVICEMONEYCOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>

class CommandsTabController;

class DeviceMoneyCommandsTabController : public AbstractTabController
{
public:
    DeviceMoneyCommandsTabController(Ui::MainWindow *ptr, QWidget *parent);

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * чтения регистров устройства.
     */
    void onReadButtonPressed();

private:
    CommandsTabController* commandsTabController();

    std::vector<std::string> m_registerNames;
};


#endif //FRTESTER_DEVICEMONEYCOMMANDSTABCONTROLLER_H
