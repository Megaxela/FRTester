//
// Created by megaxela on 13.11.16.
//

#ifndef FRDRVCROSS_INTERFACE_H
#define FRDRVCROSS_INTERFACE_H

#include <cstdint>
#include <memory>
#include "../Tools/ByteArray.h"

class PhysicalInterface;

typedef std::shared_ptr<PhysicalInterface> InterfacePtr;

/**
 * @brief Абстрактный класс, описывающий интерфейс соединения
 * с фискальным регистратором.
 */
class PhysicalInterface
{
public:

    static const int Type;

    typedef uint32_t size_t;

    /**
     * @brief Конструктор.
     */
    PhysicalInterface(int t);

    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~PhysicalInterface();

    /**
     * @brief Метод для открытия соединения.
     * @return Результат открытия соединения.
     */
    virtual bool openConnection() = 0;

    /**
     * @brief Метод для закрытия соединения.
     * @return Результат закрытия соединения.
     */
    virtual bool closeConnection() = 0;

    /**
     * @brief Метод для отправки данных по интерфейсу.
     * @param data Передаваемые данные.
     * @return Количество переданных данных.
     */
    virtual size_t write(const ByteArray &data) = 0;

    /**
     * @brief Метод для чтения определенного набора байт
     * из интерфейса с заданным таймаутом.
     * @param n Количество считываемых байт.
     * @param timeout Таймаут в миллисекундах.
     * @return Считанный массив байт.
     */
    virtual ByteArray read(const size_t &n, uint32_t timeout) = 0;

    /**
     * @brief Actual interface type.
     * @return Number of actual interface.
     */
    int type() const;

private:

    int m_t;
};


#endif //FRDRVCROSS_INTERFACE_H
