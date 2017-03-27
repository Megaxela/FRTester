//
// Created by megaxela on 11.01.17.
//

#ifndef FRTESTER_DEVICEACTIONSCOMMANDSTABCONTROLLER_H
#define FRTESTER_DEVICEACTIONSCOMMANDSTABCONTROLLER_H


#include <include/Windows/Controllers/AbstractTabController.h>

class CommandsTabController;

/**
 * @brief Класс, управляющий вкладкой с действиями устройства.
 */
class DeviceActionsCommandsTabController : public AbstractTabController
{
public:
    DeviceActionsCommandsTabController(Ui::MainWindow* ptr, QWidget* parent);

public slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку пинга.
     */
    void onPingButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * выполнения кастомной команды.
     */
    void onCommandRequestButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * перезапуска фискального регистратора.
     */
    void onRebootButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * записи нового заводского номера.
     */
    void onFirmwareNumberWriteButtonPressed();

protected:
    void setupConnections() override;

    void configureWidgets() override;

private:
    CommandsTabController* commandsTabController();
};


#endif //FRTESTER_DEVICEACTIONSCOMMANDSTABCONTROLLER_H
