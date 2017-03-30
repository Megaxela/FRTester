//
// Created by Megaxela on 22.03.2016.
//

#ifndef MIDRVCROSS_TTY_H
#define MIDRVCROSS_TTY_H

#include "Platform.h"
#include <string>
#include "ByteArray.h"
#ifdef OS_WINDOWS
#include <windows.h>
#endif

/**
 * @brief Класс описывающий TTY соединение.
 */
class TTY
{
public:

    typedef long long time_t;

    /**
     * @brief Констуктор.
     */
    TTY();

    /**
     * @brief Виртуальный деструтор.
     * (для наследования)
     */
    virtual ~TTY();

    /**
     * @brief Метод для проверки наличия соединения
     * с устройством.
     * @return Результат инициализации.
     */
    bool isInitialized() const;

    /**
     * @brief Метод установки соединения с устройством.
     * @param[in] port Название порта. Для Windows - COM порт. Для Linux - /dev интерфейс.
     * @return Результат подключения к устройству.
     */
    int connect(const std::string &port, int baudrate);

    /**
     * @brief Метод разрыва соединения с устройством.
     */
    void disconnect();

    /**
     * @brief Установка времени ожидания после соединения.
     * @param[in] ms Время в миллисекундах.
     */
    void setAfterConnectionDelay(int ms);

    /**
     * @brief Получение времени ожидания после соединения в миллисекундах.
     * @return Количество миллисекунд ожидания после соединения.
     */
    int afterConnectionDelay();

    /**
     * @brief Метод получения времени в миллисекундах.
     */
    static TTY::time_t getTimeMs();
protected:

    /**
     * @brief Метод записи на устройство.
     */
    uint32_t write(const ByteArray &dataArray) const;

    /**
     * @brief Метод чтения с устройства.
     * @param[in] size Количество считываемых данных
     */
    ByteArray read(uint32_t size, uint32_t timeoutMcs) const;

    /**
     * @brief Метод для выполнения ожидания в миллисекундах
     * @param[in] ms Время в миллисекундах.
     */
    void sleep(int ms) const;
private:
    /**
     * @brief Удаленный копирующий конструктор.
     */
    TTY(const TTY &) = delete;

    /**
     * @brief Удаленный копирующий оператор присваивания.
     */
    TTY &operator=(const TTY &) = delete;

    /**
     * Член, описывающий время ожидания перед работой после
     * успешного подключения.
     * В миллисекундах.
     */
    int m_afterConnectionWaitingTime;

#ifdef OS_WINDOWS
    mutable HANDLE m_Handle;
#endif
#ifdef OS_LINUX
    int m_fileDescriptor;
#endif
};

/**
 * @brief Кастомное исключение.
 */
struct TTYException : public std::exception
{
    /**
     * @brief Конструктор.
     * @param[in] __arg Ошибка.
     */
    TTYException(const std::string &__arg) :
            m_str(__arg)
    {}

    /**
     * @brief Виртуальный метод получения ошибки.
     */
    virtual const char *what() const noexcept override
    {
        return m_str.c_str();
    }

    /**
     * Ошибка.
     */
    std::string m_str;
};


#endif //MIDRVCROSS_TTY_H
