//
// Created by megaxela on 05.12.16.
//

#ifndef FRTESTER_SETTINGSCONTROLLER_H
#define FRTESTER_SETTINGSCONTROLLER_H

#include "AbstractTabController.h"

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Контроллер вкладки с настройками.
 */
class SettingsController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на графический интерфейс.
     * @param parent Указатель на родитеский объект.
     */
    SettingsController(Ui::MainWindow* ptr, QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~SettingsController() override;

    /**
     * @brief Метод, вызываемый при выборе вкладки,
     * управляемой этим контроллером.
     */
    void tabSelected() override;

    /**
     * @brief Метод, вызываемый при выходе из вкладки,
     * управляемой этим контроллером.
     */
    void tabLeaved() override;

protected:
    /**
     * @brief Метод, вызываемый на этапе настройки
     * соединений с объектами.
     */
    void setupConnections() override;

    /**
     * @brief Метод, вызываемый на этапе настройки
     * виджетов.
     */
    void configureWidgets() override;

private slots:
    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * выбора папки с тестами.
     */
    void onTestsPathPushButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * выбора папки с триггерами.
     */
    void onTriggersPathPushButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * выбора папки с ручными тестами.
     */
    void onManualTestsPathPushButtonPressed();
};


#endif //FRTESTER_SETTINGSCONTROLLER_H
