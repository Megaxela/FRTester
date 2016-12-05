//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_MAINWINDOW_H
#define FRTESTER_MAINWINDOW_H

#include <QMainWindow>
#include <include/Windows/Controllers/AbstractTabController.h>
#include <QMap>

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Класс главного окна.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Явный конструктор.
     * @param parent Родитель.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Деструктор.
     */
    ~MainWindow();

private slots:
    void onCurrentTabChanged(int index);

private:
    void addTabController(QWidget* tabWidget, AbstractTabController* controller);

    QMap<QWidget*, AbstractTabController*> m_tabControllers;
    QWidget* m_previousTab;

    Ui::MainWindow* m_ui;
};


#endif //FRTESTER_MAINWINDOW_H
