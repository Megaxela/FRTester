//
// Created by megaxela on 08.12.16.
//

#ifndef FRTESTER_ROOTCONTROLLER_H
#define FRTESTER_ROOTCONTROLLER_H

#include "AbstractTabController.h"

/**
 * @brief Класс, описывающий корневой контроллер.
 */
class RootController : public AbstractTabController
{
public:
    explicit RootController(Ui::MainWindow *ptr,
                            QWidget *parent,
                            QTabWidget* tabWidget);

    ~RootController() override;

protected:
    void setupConnections() override;

    void configureWidgets() override;

private slots:
    void onTabChanged(int);

};


#endif //FRTESTER_ROOTCONTROLLER_H
