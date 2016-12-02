//
// Created by megaxela on 02.12.16.
//

#ifndef FRTESTER_TESTLOGGERWAITER_H
#define FRTESTER_TESTLOGGERWAITER_H

#include <QThread>
#include <mutex>

/**
 * @brief Класс, следящий за тестировочным
 * логгером.
 */
class TestLoggerWaiter : public QThread
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     */
    TestLoggerWaiter();

    /**
     * @brief Деструктор.
     */
    ~TestLoggerWaiter();

    /**
     * @brief Метод для остановки работы.
     */
    void stopWorking();
signals:
    /**
     * @brief Сигнал, вызываемый при получении
     * нового лога.
     * @param string Лог.
     */
    void logReceived(QString string);

protected:
    void run() override;

private:
    std::mutex m_mutex;

    bool m_running;
};


#endif //FRTESTER_TESTLOGGERWAITER_H
