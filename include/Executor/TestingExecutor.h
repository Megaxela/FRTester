//
// Created by megaxela on 22.11.16.
//

#ifndef FRTESTER_TESTINGEXECUTOR_H
#define FRTESTER_TESTINGEXECUTOR_H

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <condition_variable>
#include "Testing/AbstractTest.h"
#include "Testing/AbstractTriggerTest.h"

/**
 * @brief Метод управляющий выполнением
 * тестировочного ядра и связывает его с
 * графикой.
 */
class TestingExecutor : public QThread
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     */
    TestingExecutor();

    /**
     * @brief Деструктор.
     */
    ~TestingExecutor();

    /**
     * @brief Метод для проверки идет ли сейчас
     * тестирование.
     */
    bool isTestingRunning();

    /**
     * @brief Метод для проверки установлена ли
     * сейчас пауза.
     */
    bool isPaused();

public slots:
    /**
     * @brief Слот для временной остановки
     * тестирования.
     */
    void pause();

    /**
     * @brief Слот для продолжения тестирования
     * после временной остановки.
     */
    void resume();

    /**
     * @brief Слот для прекрещения тестирования.
     */
    void stop();

protected:
    void run();

signals:
    /**
     * @brief Сигнал, вызываемый при провале во время тестирования.
     * Таким провалом может быть невозможность восстановить
     * начальное состояние устройства.
     * @param reason
     */
    void testingFailed(QString reason);

    /**
     * @brief Сигнал, вызываемый при ручном прекращении
     * тестирования.
     */
    void testingStopped();

    /**
     * @brief Сигнал, вызываемый при окончании тестирования.
     */
    void testingFinished();

    /**
     * @brief Сигнал, вызываемый при временной остановке
     * тестирования.
     */
    void testingPaused();

    /**
     * @brief Сигнал, вызываемый при продолжении тестирования
     * после временной остановки.
     */
    void testingResumed();

    /**
     * @brief Сигнал, вызываемый при начале тестирования.
     */
    void testingStarted();

    /**
     * @brief Сигнал, вызываемый при получении результата
     * выполненного теста.
     * @param test Указатель на тест.
     * @param result Результат выполнения теста.
     */
    void testResultAcquired(TestPtr test, bool result);

    /**
     * @brief Сигнал, вызываемый при проверке триггеров,
     * вернувших ошибку во время выполнения теста.
     * @param trigger Указатель на триггер.
     */
    void triggerFailAcquired(TriggerTestPtr trigger);

    /**
     * @brief Сигнал вызываемый при необходимости сообщить
     * некоторую полезную информацию о прохождении тестов
     * наружу.
     * @param log Сообщение информации.
     */
    void testingLogAcquired(QString log);

private:
    std::mutex m_mutex;

    bool m_running;

    bool m_paused;

    bool m_passthroughTesting;

    std::condition_variable m_notifier;
};


#endif //FRTESTER_TESTINGEXECUTOR_H
