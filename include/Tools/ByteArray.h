//
// Created by Megaxela on 30.12.2015.
//

#ifndef CLIENTSERVER_BYTEARRAY_H
#define CLIENTSERVER_BYTEARRAY_H


#include <stdint.h>
#include "SystemTools.h"
#include <sstream>

#include <iostream>
#include <memory.h>
#include <assert.h>

/**
 * @brief Template class for byte arrays.
 * @todo Translate documentation.
 * @todo Add tests for this class.
 */
class ByteArray
{
    /**
     * @brief Перегруженная функция вывода в поток.
     */
    friend std::ostream &operator<<(std::ostream &ostream, const ByteArray &arr);

public:
    /**
     * Перечисляемый тип, описывающий порядок байт.
     */
    enum ByteOrder
    {
        ByteOrder_BigEndian,    ///< Порядок от старшего к младшему (Big-endian)
        ByteOrder_LittleEndian  ///< Порядок от младшего к старшему (Little-endian)
    };

    /**
     * @brief Конструктор.
     */
    ByteArray();

    /**
     * @brief Constructor with initial capacity.
     * @param capacity Initial capacity.
     */
    ByteArray(uint32_t capacity);

    /**
     * @brief Конструктор из уже готовый байт.
     * @param array Массив байт.
     * @param size Размер массива.
     */
    ByteArray(const uint8_t* array, uint32_t size);

    /**
     * @brief Копирующий конструктор.
     */
    ByteArray(const ByteArray &rhs);

    /**
     * @brief Деструктор.
     */
    ~ByteArray();

    /**
     * @brief Метод для добавления к массиву байт другой массив байт.
     * @param array Добавляемый массив.
     */
    void append(const ByteArray &array);

    /**
     * @brief Метод для добавления к массиву байт другой массив байт.
     * @param array Массив байт.
     * @param size Размер массива.
     */
    void append(const uint8_t* array, uint32_t size);

    void appendPart(uint64_t number, uint32_t size, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(sizeof(uint64_t) >= size);

        if (m_n + size > m_c)
        {
            changeCapacity(m_n + size);
        }

        for (uint32_t i = 0;
             i < size;
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[m_n + i] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl<uint64_t>(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
            else
            {
                m_d[m_n + size - i - 1] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl<uint64_t>(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
        }

        m_n += size;
    }

    /**
     * @brief Шаблонный метод для множественного добавления
     * элементов к массиву.
     * @param number Добавляемое число типа T
     * @param count Количество добавляемых чисел
     * @param order Порядок байт, использующийся при добавлении числа
     */
    template<typename T>
    void appendMultiple(T number, uint32_t count, ByteOrder order = ByteOrder_BigEndian)
    {
        if (m_n + sizeof(T) * count > m_c)
        {
            changeCapacity(m_n + sizeof(T) * count);
        }

        for (int i = 0;
             i < static_cast<int>(count);
             ++i)
        {
            append(number, order);
        }
    }

    /**
     * @brief Шаблонный класс для добавления значений к массиву.
     */
    template<typename T>
    void append(T number, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(order == ByteOrder_LittleEndian || order == ByteOrder_BigEndian);

        if (m_n + sizeof(T) > m_c)
        {
            changeCapacity(m_n + sizeof(T));
        }

        for (int i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[m_n + i] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl<T>(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
            else
            {
                m_d[m_n + sizeof(T) - i - 1] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl<T>(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
        }

        m_n += sizeof(T);
    }

    /**
     * @brief Метод для вставки массива в текущий массив.
     * @param position Место вставки.
     * @param array Массив для вставки.
     */
    void insert(uint32_t position, const ByteArray &array);

    /**
     * @brief Метод для вставки массива в текущий массив.
     * @param position Место вставки.
     * @param array Массив для вставки.
     * @param size Размер массива.
     */
    void insert(uint32_t position, const uint8_t* array, uint32_t size);

    /**
     * @brief Метод для вставки числа в текущий массив.
     * @tparam T Тип вставляемого числа. На его основе будут расчитанны смещения.
     * @param position Позиция вставки.
     * @param number Вставляемое число.
     * @param order Порядок байт в котором число будет вставлено.
     */
    template<typename T>
    void insert(uint32_t position, T number, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(position < m_n);

        if (m_n + sizeof(T) > m_c)
        {
            changeCapacity(m_n + 4);
        }

        for (uint32_t index = m_n - 1; index >= position; --index)
        {
            m_d[index + sizeof(T)] = m_d[index];
        }

        for (int i = 0; i < sizeof(T); ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[position + i] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
            else
            {
                m_d[position + sizeof(T) - i - 1] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
        }

        m_n += 4;
    }

    /**
     * @brief Метод для установки в значение числа определенного набора байт.
     * @tparam T Тип вставляемого числа. На его основе будут расчитанны смещения.
     * @param position Позиция.
     * @param number Значение в которое будет установлен набор байт.
     * @param order Порядок байт в котором будет осуществляться установка.
     */
    template<typename T>
    void set(uint32_t position, T number, ByteOrder order = ByteOrder_BigEndian)
    {
        assert(position + (sizeof(T) - 1) < m_n);
        for (int i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                m_d[position + i] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
            else
            {
                m_d[position + sizeof(T) - i - 1] = static_cast<uint8_t>(
                        SystemTools::shr(
                                number & SystemTools::shl(
                                        0xFF,
                                        i * 8
                                ),
                                i * 8
                        )
                );
            }
        }
    }

    /**
     * @brief Метод для считывания числа из набора байт.
     * @tparam T Тип считываемого числа. На его основе будут расчитанны смещения.
     * @param position Место с которого будет считываться число.
     * @param order Последовательность байт в которой будет считываться число.
     * @return Результат считывания. Число. 
     */
    template<typename T>
    T read(uint32_t position, ByteOrder order = ByteOrder_BigEndian) const
    {
        assert(order == ByteOrder_LittleEndian || order == ByteOrder_BigEndian);
        assert((position + sizeof(T)) <= length());

        T value = T(0);

        for (int i = 0;
             i < static_cast<int>(sizeof(T));
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                value |= SystemTools::shl<T>(m_d[position + i], i * 8);
            }
            else
            {
                value |= SystemTools::shl<T>(m_d[position + sizeof(T) - i - 1], i * 8);
            }
        }

        return value;
    }

    uint64_t readPart(uint32_t position, uint8_t size, ByteOrder order = ByteOrder_BigEndian) const
    {
        assert(order == ByteOrder_LittleEndian || order == ByteOrder_BigEndian);
        assert((position + size) <= length());

        uint64_t value = uint64_t(0);

        for (int i = 0;
             i < static_cast<int>(size);
             ++i)
        {
            if (order == ByteOrder_LittleEndian)
            {
                value |= SystemTools::shl(m_d[position + i], i * 8);
            }
            else
            {
                value |= SystemTools::shl(m_d[position + size - i - 1], i * 8);
            }
        }

        return value;
    }

    /**
     * @brief Статический метод для получения массива байт из 
     * строкового представления массива байт.
     * @param hex 
     * @return 
     */
    static ByteArray fromHex(const std::string &hex);

    /**
     * @brief Метод для получения размера массива.
     */
    uint32_t length() const;

    /**
     * @brief Метод для получения размера массива.
     */
    uint32_t size() const;

    /**
     * @brief Метод для проверки наличия данных в массиве.
     */
    bool empty() const;

    /**
     * @brief Метод для получения размера контейнера массива.
     */
    uint32_t capacity() const;

    /**
     * @brief Метод для получения указателя на данные.
     */
    const uint8_t* data() const;

    /**
     * @brief Метод для получения перевернутой версии массива байт.
     */
    ByteArray reversed() const;

    /**
     * @brief Метод вырезания части массива.
     * @param start Начальная позиция.
     * @param end Конечная позиция.
     */
    ByteArray cut(unsigned int start, unsigned int end) const;

    /**
     * @brief Метод для изменения размера контейнера.
     * @param c Новый размер.
     */
    void changeCapacity(uint32_t c);

    /**
     * @brief Метод для получения данных по индексу.
     * @param i Индекс.
     */
    uint8_t &operator[](unsigned int i);

    /**
     * @brief Метод для получения данных по индексу.
     * @param i Индекс.
     */
    uint8_t  operator[](unsigned int i) const;

    /**
     * @brief Копирующий оператор присваивания.
     * @param array Копируемый объект.
     */
    ByteArray &operator=(const ByteArray &array);

    /**
     * @brief Метод преобразования байт в строку, разделенную пробелами.
     */
    std::string toHex() const;

    /**
     * @brief Метод получения подмассива из текущего массива.
     * @param begin Начало массива.
     * @param length Длина подмассива.
     * @return Подмассив.
     */
    ByteArray mid(uint32_t begin, uint32_t length) const;

    /**
     * @brief Оператор сравнения.
     * @param array Сравниваемый объект.
     */
    bool operator==(const ByteArray &array) const;

    /**
     * @brief Оператор сравнения.
     * @param array Сравниваемый объект.
     */
    bool operator!=(const ByteArray &array) const;

private:

    /**
     * Указатель на массив байт.
     */
    uint8_t *m_d;

    /**
     * Полезная часть массива.
     */
    uint32_t m_n;

    /**
     * Размер контейнера.
     */
    uint32_t m_c;
};


#endif //CLIENTSERVER_BYTEARRAY_H