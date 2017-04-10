//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_COMMANDSTABCONTROLLER_H
#define FRTESTER_COMMANDSTABCONTROLLER_H


#include <QMap>
#include <Testing/ConnectionsManager/Connection.h>
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
    Q_OBJECT

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

    /**
     * @brief Метод для проверки наличия соединения. Если
     * соединения не будет - то будет отображено модальное окно.
     * @return Наличие соединения.
     */
    bool checkConnectionWithDevice();

signals:
    /**
     * @brief Сигнал, сообщающий о том, что было добавлено
     * новое соединение.
     * @param connection Умный указатель на объект соединения.
     */
    void connectionAdded(std::shared_ptr<Connection> connection);

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
