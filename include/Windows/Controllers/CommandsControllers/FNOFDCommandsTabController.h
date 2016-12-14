//
// Created by megaxela on 13.12.16.
//

#ifndef FRTESTER_FNOFDCOMMANDSTABCONTROLLER_H
#define FRTESTER_FNOFDCOMMANDSTABCONTROLLER_H


#include <include/Windows/Controllers/AbstractTabController.h>

class CommandsTabController;

/**
 * @brief Класс, контроллирующий вкладку ОФД в виджете из вкладки
 * ФН в разделе с командами.
 */
class FNOFDCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский виджет.
     */
    FNOFDCommandsTabController(Ui::MainWindow *ptr, QWidget *parent);

    /**
     * @brief Метод для получения родителя - контроллера команд.
     * @return Указатель на контроллер.
     */
    CommandsTabController* commandsTabController() const;

protected:
    /**
     * @brief Метод для настройки соединений.
     */
    void setupConnections() override;

    /**
     * @brief Метод для настройки виджетов.
     */
    void configureWidgets() override;

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * получения статуса информационного обмена.
     */
    void onGetExchangeStatus();

private:
    QString yesNo(int v);

    QString dateTime(uint8_t day, uint8_t mon, uint8_t year, uint8_t hour, uint8_t min);
};


#endif //FRTESTER_FNOFDCOMMANDSTABCONTROLLER_H
