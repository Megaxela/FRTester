//
// Created by megaxela on 02.12.16.
//

#ifndef FRTESTER_TESTLOGGER_H
#define FRTESTER_TESTLOGGER_H

#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>

/**
 * @brief Класс, описывающий тестировочный логгер.
 */
class TestLogger
{
public:
    /**
     * @brief Деструктор.
     */
    ~TestLogger();

    /**
     * @brief Метод для получения
     * @return
     */
    static TestLogger& instance();

    /**
     * @brief Метод логгирования
     * @param string
     */
    void log(const std::string &string);

    /**
     * @brief Метод для ожидания
     */
    void waitForData();

    /**
     * @brief Метод для получения и выталкивания
     * последнего сообщения.
     * @return
     */
    std::string popMessage();

    /**
     * @brief Метод для проверки наличия данных в
     * логгере.
     * @return
     */
    bool hasData() const;

private:
    TestLogger();

    std::condition_variable m_notifier;
    mutable std::mutex m_mutex;
    bool m_finished;

    std::queue<std::string> m_messages;
};


#endif //FRTESTER_TESTLOGGER_H
