//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_TESTINGEXECUTOR_H
#define FRTESTER_TESTINGEXECUTOR_H

#include <QtCore/QObject>

/**
 * @brief Метод управляющий выполнением
 * тестировочного ядра и связывает его с
 * графикой.
 */
class TestingExecutor : public QObject
{
    Q_OBJECT

public:
    TestingExecutor();

    ~TestingExecutor();

signals:
    
};


#endif //FRTESTER_TESTINGEXECUTOR_H
