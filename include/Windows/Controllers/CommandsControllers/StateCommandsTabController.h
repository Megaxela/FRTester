//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_STATECOMMANDSTABCONTROLLER_H
#define FRTESTER_STATECOMMANDSTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <include/Windows/Controllers/CommandsTabController.h>

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс, контроллирующий вкладку команд с набором действий
 * по работе с запросми состояний.
 */
class StateCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский виджет.
     */
    StateCommandsTabController(Ui::MainWindow *ptr,
                               QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~StateCommandsTabController() override;

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
     * @brief Слот, вызываемый при запросе полного состояния ФР.
     */
    void onFullRequest();

    /**
     * @brief Слот, вызываемый при запроса краткого
     */
    void onShortRequest();

    /**
     * @brief Слот, вызываемый при запросе необнуляемых сумм.
     */
    void onZeroSumsRequest();

private:
    /**
     * @brief Метод для преобразования даты в строку.
     * @param date Объект даты.
     * @return Строковое представление даты.
     */
    QString stateDateToString(const FRDriver::DateStructure& date);

    /**
     * @brief Метод для преобразования времени в строку.
     * @param time Объект времени.
     * @return Строковое представление времени.
     */
    QString stateTimeToString(const FRDriver::TimeStructure& time);
};


#endif //FRTESTER_STATECOMMANDSTABCONTROLLER_H
