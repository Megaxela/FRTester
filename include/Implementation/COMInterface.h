//
// Created by megaxela on 14.11.16.
//

#ifndef FRDRVCROSS_COMINTERFACE_H
#define FRDRVCROSS_COMINTERFACE_H


#include "../Tools/TTY.h"
#include "../Abstract/PhysicalInterface.h"
#include "../Tools/Time.h"

/**
 * @brief Класс, описывающий интерфейс соединения по COM порту.
 */
class COMInterface : public PhysicalInterface,
                      protected TTY
{
public:
    const int Type = 1;

    /**
     * @brief Конструктор.
     */
    COMInterface();

    /**
     * @brief Деструктор.
     */
    ~COMInterface();

    /**
     * @brief Метод для открытия соединения по
     * COM порту.
     * @return Успешность соединения.
     */
    bool openConnection() override;

    /**
     * @brief Метод закрытия соединения по COM порту.
     * @return Успешность закрытия соединения.
     */
    bool closeConnection() override;

    /**
     * @brief Метод для передачи данных по COM порту.
     * Эти данные будут просто отправлены на устройство.
     * @param data Данные.
     * @return Количество отправленных данных.
     */
    size_t write(const ByteArray &data) override;

    /**
     * @brief Метод для установки названия порта для
     * соединения. /dev (/dev/ttyS1) интерфейс для Linux и
     * COM (COM1) порт для Windows.
     * @param name Название порта.
     */
    void setDevice(const std::string &name);

    /**
     * @brief Метод для получения названия порта для
     * соединения. См COMInterface::setDevice.
     * @return Название порта.
     */
    std::string device() const;

    /**
     * @brief Метод для установки скорости передачи данных.
     * @param rate Скорость передачи данных.
     */
    void setBaudRate(int rate);

    /**
     * @brief Метод для получения установленной скорости
     * передачи данных.
     * @return Скорость передачи данных.
     */
    int baudRate() const;

    /**
     * @brief Метод для считывания данных.
     * @param n Количество считываемых данных.
     * @param timeout Таймаут считывания.
     * @return Результат считывания.
     */
    ByteArray read(const size_t &n, uint32_t timeout) override;

    /**
     * @brief Метод для установки времени отправки одного байта.
     * @param time Время в микросекундах.
     */
    void setByteSendTime(Time::time_t time);

private:
    /**
     * Название порта для соединения.
     * /dev интерфейс для linux
     * COM порт для Windows
     */
    std::string m_device;

    /**
     * Скорость передачи данных.
     */
    int m_baudRate;

    /**
     * Время передачи одного байта. В микросекундах.
     */
    Time::time_t m_byteSendTime;

    /**
     * Время последнего отправленного байта.
     */
    Time::time_t m_lastByteSendTime;
};

/**
 * @brief Исключение интерфейса.
 */
struct COMException : public std::exception
{
    /**
     * @brief Конструктор.
     * @param __arg Ошибка.
     */
    COMException(const std::string &__arg) :
            m_str(__arg)
    {}

    /**
     * @brief Виртуальный метод для получения ошибки.
     * @return Ошибкаю
     */
    virtual const char* what() const noexcept override
    {
        return m_str.c_str();
    }

    /**
     * Сама ошибка.
     */
    std::string m_str;
};

#endif //FRDRVCROSS_COMINTERFACE_H
