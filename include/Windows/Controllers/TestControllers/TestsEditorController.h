//
// Created by megaxela on 17.02.17.
//

#ifndef FRTESTER_TESTSEDITORCONTROLLER_H
#define FRTESTER_TESTSEDITORCONTROLLER_H


#include <Windows/Controllers/AbstractTabController.h>
#include <Testing/ManualTests/ManualTest.h>

class TestsEditorController : public AbstractTabController
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор.
     * @param ptr Указатель на UI.
     * @param parent Указатель на родительский объект
     */
    TestsEditorController(Ui::MainWindow* ptr, QWidget* parent);

    /**
     * @brief Деструктор.
     */
    ~TestsEditorController();

    void tabSelected() override;

    void tabLeaved() override;

signals:

    void actionChanged(TestActionPtr action);

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:

    /**
     * @brief Слот, вызываемый при выборе теста в селекторе.
     * @param test Умный указатель на выбранный тест.
     */
    void onTestSelectorTestSelected(ManualTestPtr test);

    /**
     * @brief Слот, вызываемый при смене названия теста.
     * @param name Новое название теста.
     */
    void onTestNameChanged(QString name);

    /**
     * @brief Слот, вызываемый при смене строки поиска
     * действий теста.
     * @param name Новое значение строки поиска.
     */
    void onTestActionsSearchChanged(QString name);

    /**
     * @brief Слот, вызываемый при смене строки поиска
     * доступных действий.
     * @param name Новое значение строки поиска.
     */
    void onAvailableActionsSearchChanged(QString name);

    /**
     * @brief Слот, вызываемый при выборе элемента среди
     * доступных действия.
     * @param item Выбранный элемент. Может быть nullptr.
     */
    void onAvailableActionElementSelected(TestActionPtr item);

    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * добавления теста.
     */
    void onAddTestButtonPressed();

    /**
     * @brief Слот, вызываемый при нажатии на кнопку
     * удаления теста.
     */
    void onRemoveTestButtonPressed();

    /**
     * @brief Слот для очистки области для изменения теста.
     */
    void clearChecker();

private:

    void disconnectEditor();

    void connectEditor();

    void displayCurrentTest();

    ManualTestPtr m_currentTest;
    TestActionPtr m_editingAction;
    bool m_editorDisconnected;
};


#endif //FRTESTER_TESTSEDITORCONTROLLER_H
