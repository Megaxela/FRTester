//
// Created by megaxela on 04.04.17.
//

#ifndef FRTESTER_FNFISCALISATIONTABCONTROLLER_H
#define FRTESTER_FNFISCALISATIONTABCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>

class CommandsTabController;

/**
 * @brief Класс, описывающий контроллер для управления
 * вкладкой для фискализации.
 */
class FNFiscalisationTabController : public AbstractTabController
{
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский объект.
     */
    FNFiscalisationTabController(Ui::MainWindow *ptr, QWidget *parent);

protected:

    void setupConnections() override;

    void configureWidgets() override;

private slots:

    /**
     * @brief Слот, вызываемый при смене состояния чекбоксов
     * для режима налогов.
     * @param checked Состояние чекбокса.
     * @param orBits Бит, который нужно изменить.
     */
    void onTaxCheckboxClicked(bool checked, uint8_t orBits);

    /**
     * @brief Слот, вызываемый при смене состояния чекбоксов
     * для режима работы.
     * @param checked Состояние чекбокса.
     * @param orBits Бит, который нужно изменить.
     */
    void onWorkModeCheckboxClicked(bool checked, uint8_t orBits);

    /**
     * @brief Слот, вызываемый при нажатии на кнопку генерации
     * контрольной суммы для ИНН.
     * В этом методе происходит генерация ИНН для ЮР и ФИЗ лиц.<br>
     * Алгоритм:<br>
     * Для ЮР лиц - КС является последний символ. Алгорим вычисления: <br>
     * ((2 * n1 + 4 * n2 + 10 * n3 + 3 * n4 + 5 * n5 + 9 * n6 + 4 * n7 + 6 * n8 + 8 * n9) mod 11) mod 10<br>
     * Для ФИЗ лиц - КС являются последние 2 символа. Алгорим вычисления: <br>
     * n11 = ((7 * n1 + 2 * n2 + 4 * n3 + 10 * n4 + 3 * n5 + 5 * n6 + 9 * n7 + 4 * n8 + 6 * n9 + 8 * n10) mod 11) mod 10<br>
     * n12 = ((3 * n1 + 7 * n2 + 2 * n3 + 4 * n4 + 10 * n5 + 3 * n6 + 5 * n7 + 9 * n8 + 5 * n9 + 6 * n10 + 8 * n11) mod 11) mod 10<br>
     * Где nx - число в ИНН в позиции X
     */
    void onGenerateINNClicked();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку генерации
     * регистрационного номера ККТ.
     */
    void onGenerateRNMClicked();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку регистрации.
     */
    void onRegistrationButtonClicked();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку перерегистрации.
     */
    void onReRegistrationButtonClicked();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку очистки
     * состояния ФН.
     */
    void onResetButtonClicked();

private:

    /**
     * @brief Метод для получения родителя - контроллера команд.
     * @return Указатель на контроллер.
     */
    CommandsTabController* commandsTabControllerParent() const;

    uint8_t m_taxMode;
    uint8_t m_workMode;
};


#endif //FRTESTER_FNFISCALISATIONTABCONTROLLER_H
