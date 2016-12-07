//
// Created by megaxela on 07.12.16.
//

#ifndef FRTESTER_PRINTCOMMANDSTABCONTROLLER_H
#define FRTESTER_PRINTCOMMANDSTABCONTROLLER_H


#include <include/Windows/Controllers/AbstractTabController.h>
#include <QMap>

class CommandsTabController;

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс, контроллирующий вкладку c командами печати.
 */
class PrintCommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский виджет.
     * @param parentController Указатель на родительский контроллер.
     */
    PrintCommandsTabController(Ui::MainWindow* ptr,
                               QWidget* parent,
                               AbstractTabController* parentController=nullptr);

    /**
     * @brief Деструктор.
     */
    ~PrintCommandsTabController();

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
     * @brief Слот, вызываемый при смене вкладки.
     */
    void onCurrentTabChanged(int);

private:
    void addTabController(QWidget* widget, AbstractTabController* el);

    Ui::MainWindow* m_ui;

    QWidget* m_parent;

    CommandsTabController* m_parentController;

    QMap<QWidget*, AbstractTabController*> m_tabControllers;

    QWidget* m_previousTab;
};


#endif //FRTESTER_PRINTCOMMANDSTABCONTROLLER_H
