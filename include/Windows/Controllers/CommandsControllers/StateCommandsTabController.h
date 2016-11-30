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
                               QWidget* parent,
                               AbstractTabController* parentController=nullptr);

    /**
     * @brief Деструктор.
     */
    ~StateCommandsTabController() override;

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

private:
    Ui::MainWindow* m_ui;

    QWidget* m_parent;

    CommandsTabController* m_parentController;
};


#endif //FRTESTER_STATECOMMANDSTABCONTROLLER_H
