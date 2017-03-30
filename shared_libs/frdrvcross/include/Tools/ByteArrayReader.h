//
// Created by megaxela on 13.12.16.
//

#ifndef FRTESTER_BYTEARRAYREADER_H
#define FRTESTER_BYTEARRAYREADER_H

#include "ByteArray.h"
#include "Logger.h"

/**
 * @brief Класс для последовательного чтения данных из
 * массива байт.
 * Работает долько до тех пор, пока активен считываемый объект.
 * Копирования не производит.
 */
class ByteArrayReader
{
public:
    /**
     * @brief Конструктор.
     * @param reader Считываемый объект.
     */
    ByteArrayReader(const ByteArray& reader);

    /**
     * @brief Метод для перемещения текущего указателя
     * по данным.
     * @param pos Место для перемещения.
     */
    void seek(uint32_t pos);

    /**
     * @brief Метод для сдвига текущего указателя по данным.
     * @param val Значение сдвига.
     */
    void move(int val);

    /**
     * @brief Метод для считывания числа из набора байт.
     * @tparam T Тип считываемого числа. На его основе будут расчитанны смещения.
     * @param order Последовательность байт в которой будет считываться число.
     * @return Результат считывания. Число.
     */
    template< typename T >
    T read(ByteArray::ByteOrder order)
    {
        T val = m_reader.read<T>(m_pos, order);

        m_pos += sizeof(T);

        return val;
    }

    /**
     * @brief Метод для считывания части числа.
     * @param size Количество считываемых данных.
     * @param order Последовательность байт в которой будут считываться числа.
     * @return Результат считывания. Число.
     */
    uint64_t readPart(uint8_t size, ByteArray::ByteOrder order);

    /**
     * @brief Метод для считывания строки.
     * @param size Длина считываемой строки.
     * @return Результат считывания. Строка.
     */
    std::string readString(uint32_t size);

private:
    uint32_t m_pos;
    const ByteArray& m_reader;
};


#endif //FRTESTER_BYTEARRAYREADER_H
