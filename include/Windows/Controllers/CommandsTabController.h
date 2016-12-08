//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_COMMANDSTABCONTROLLER_H
#define FRTESTER_COMMANDSTABCONTROLLER_H


#include <QMap>
#include "AbstractTabController.h"
#include "FRDriver.h"

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Контроллер вкладки с командами.
 */
class CommandsTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский объект.
     */
    CommandsTabController(Ui::MainWindow* ptr,
                          QWidget* parent,
                          QTabWidget* tabWidget);

    /**
     * @brief Деструктор.
     */
    ~CommandsTabController() override;

    /**
     * @brief Метод для получения пароля, заданного для всей вкладки.
     * @return Пароль.
     */
    uint32_t password() const;

    /**
     * @brief Метод для установки результата выполнения команды.
     * @param status Текстовый результат.
     */
    void setStatus(FRDriver::ErrorCode code, const QString &status);

    /**
     * @brief Метод для установки в качестве статуса результат
     * выполнения последней команды.
     */
    void setLastStatus();

protected:
    /**
     * @brief Метод, настройки коннектов.
     */
    void setupConnections() override;

    /**
     * @brief Настройка виджетов.
     */
    void configureWidgets() override;

};


#endif //FRTESTER_COMMANDSTABCONTROLLER_H
