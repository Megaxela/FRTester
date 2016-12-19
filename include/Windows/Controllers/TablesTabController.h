//
// Created by megaxela on 13.12.16.
//

#ifndef FRTESTER_TABLESTABCONTROLLER_H
#define FRTESTER_TABLESTABCONTROLLER_H


#include <include/Windows/Controllers/AbstractTabController.h>
#include <include/FRDriver.h>
#include <QtWidgets/QListWidgetItem>

/**
 * @brief Класс, управляющий вкладкой с таблицами.
 */
class TablesTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на графический интерфейс.
     * @param parent Указатель на родительский виджет.
     */
    TablesTabController(Ui::MainWindow *ptr, QWidget *parent);

protected:
    /**
     * @brief Метод для настройки соединений сигналов и слотов.
     */
    void setupConnections() override;

    /**
     * @brief Метод для настройки виджетов.
     */
    void configureWidgets() override;

private slots:
    /**
     * @brief Слот, вызываемый при обновлении таблиц.
     */
    void onTablesUpdate();

    /**
     * @brief Слот, вызываемый при смене текущей таблицы
     */
    void onCurrentTableChanged(int);

private:
    bool checkPassword() const;

    uint32_t getPassword() const;

    bool checkEverything() const;

    std::array<uint8_t, 24> m_tables;
    std::array<FRDriver::TableStructure, 24> m_tableStructures;
    std::vector<FRDriver::FieldStructure> m_currentTableFieldsStructures;
};


#endif //FRTESTER_TABLESTABCONTROLLER_H
