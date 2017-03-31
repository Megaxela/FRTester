//
// Created by megaxela on 31.03.17.
//

#ifndef FRTESTER_FNSTATECOMMANDSTABCONTROLLER_H
#define FRTESTER_FNSTATECOMMANDSTABCONTROLLER_H

#include "AbstractTabController.h"

namespace Ui
{
    class MainWindow;
}

class CommandsTabController;

/**
 * @brief Класс для контроллирования вкладки с информацией о
 * статусе фискального накопителя.
 */
class FNStateCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на интерфейс.
     * @param parent Родительский виджет.
     * @param tabWidget Подконтрольный виджет
     */
    FNStateCommandsTabController(Ui::MainWindow* ptr,
                                 QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~FNStateCommandsTabController();

protected:
    virtual void setupConnections();

    virtual void configureWidgets();

private slots:
    void onGetFNState();

private:
    QString stringifyBool(bool val);

    QString dateTime(uint8_t day, uint8_t mon, uint8_t year, uint8_t hour, uint8_t min, uint8_t seconds);

    CommandsTabController* commandsTabController() const;
};


#endif //FRTESTER_FNSTATECOMMANDSTABCONTROLLER_H
