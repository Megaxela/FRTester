//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_ABSTRACTTABCONTROLLER_H
#define FRTESTER_ABSTRACTTABCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QTabWidget>

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс, контроллирующий вкладку
 * меню.
 */
class AbstractTabController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     */
    explicit AbstractTabController(Ui::MainWindow* ptr,
                                   QWidget* parent,
                                   QTabWidget* tabWidget=nullptr) ;

    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~AbstractTabController();

    /**
     * @brief Метод, вызываемый при выборе вкладки, контроллируемой
     * реализацией контроллера.
     */
    virtual void tabSelected();

    /**
     * @brief Метод, вызываемый при покидании вкладки, контроллируемой
     * реализации контроллера.
     */
    virtual void tabLeaved();

    /**
     * @brief Метод, для инициализации контроллера.
     */
    virtual void init();

    /**
     * @brief Метод для получения родительского контроллера.
     * @return Указатель на родительский контроллер.
     */
    AbstractTabController* parentController() const;

    /**
     * @brief Метод для получения UI.
     * @return Указатель на пользовательский интерфейс.
     */
    Ui::MainWindow* ui() const;

    /**
     * @brief Метод для получения родительского виджета.
     * @return Родительский виджет.
     */
    QWidget* parentWidget() const;

    /**
     * @brief Метод для получения подконтрольного виджета.
     * @return
     */
    QTabWidget* tabWidget() const;

protected:

    void addTabController(QWidget* widget, AbstractTabController *controller);

    /**
     * @brief Метод для настройки соединений сигналов со слотами.
     */
    virtual void setupConnections() = 0;

    /**
     * @brief Метод для настройки виджетов.
     */
    virtual void configureWidgets() = 0;

private slots:
    void onCurrentTabChanged(int);

private:
    /**
     * @brief Метод для настройки локальных соединений.
     */
    void setupLocalConnections();

    /**
     * @brief Метод для установки родительского контроллера.
     * @param controller Родительский контроллер.
     */
    void setParentController(AbstractTabController* controller);

    QMap<QWidget*, AbstractTabController*> m_tabControllers;

    QWidget* m_previousTab;

    Ui::MainWindow* m_ui;

    QWidget* m_parent;

    QTabWidget* m_tabWidget;

    AbstractTabController* m_controllerParent;
};


#endif //FRTESTER_ABSTRACTTABCONTROLLER_H
