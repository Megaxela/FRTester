//
// Created by megaxela on 20.02.17.
//

#ifndef FRTESTER_QMANUALTESTSLISTWIDGET_H
#define FRTESTER_QMANUALTESTSLISTWIDGET_H

#include <QListWidget>
#include <Testing/ManualTests/ManualTest.h>

class QManualTestsListWidget : public QListWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     */
    QManualTestsListWidget(QWidget* parent);

    /**
     * @brief Деструктор.
     */
    virtual ~QManualTestsListWidget();

    /**
     * @brief Обновить тесты.
     */
    void updateTests();

    /**
     * @brief Метод для сохранения тестов обратно в файлы.
     */
    void saveTests();

    /**
     * @brief Метод для отображения тестов.
     */
    void displayTests();

    /**
     * @brief Метод для добавления теста.
     * @param test Новый тест.
     */
    void addTest(ManualTestPtr test);

    /**
     * @brief Метод для удаления теста.
     * @param index Индекс удаляемого теста.
     */
    void removeTest(uint32_t index);

signals:
    /**
     * @brief Сигнал, вызываемый при выборе теста.
     * @param test Выбранный тест. Может быть nullptr.
     */
    void onTestSelected(ManualTestPtr test);

private slots:

    /**
     * @brief Слот, вызываемый при смене выделенного
     * теста.
     */
    void onSelectionChanged();

private:

    // Пара: Прошлое название теста - тест
    std::vector<std::pair<std::string, ManualTestPtr>> m_manualTests;
};


#endif //FRTESTER_QMANUALTESTSLISTWIDGET_H
