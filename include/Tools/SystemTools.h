//
// Created by Megaxela on 18.10.2016.
//

#ifndef CARDMASTER_SYSTEMTOOLS_H
#define CARDMASTER_SYSTEMTOOLS_H

#include <cinttypes>
#include <vector>
#include <string>
#include "Tools/Platform.h"

namespace SystemTools
{
    /**
     * @brief Метод, производящий смещение числа влево.
     * Требуется по причине того, что не все процессоры
     * способны совершать смещения более чем на 31 бит.
     * @tparam T Тип данных над которым производится смещение.
     * @param v Исходное число.
     * @param offset Количество бит смещения.
     * @return Смещенное число.
     */
    template<typename T>
    T shl(T v, uint16_t offset)
    {
        if(offset > 31)
        {
            int times = offset / 31;
            for (int i = 0; i < times; ++i)
            {
                v <<= 31;
            }

            v <<= (offset - (31 * times));
        }
        else
        {
            v <<= offset;
        }

        return v;
    }

    template<typename T>
    T shr(T v, uint16_t offset)
    {
        if(offset > 31)
        {
            int times = offset / 31;
            for (int i = 0; i < times; ++i)
            {
                v >>= 31;
            }

            v >>= (offset - (31 * times));
        }
        else
        {
            v >>= offset;
        }

        return v;
    }

    /**
     * @brief Метод для получения всех файлов в директории.
     * @param path Путь к директории.
     * @return Массив с названиями файлов.
     */
    std::vector<std::string> getAllFilesInDir(const std::string &path);

    namespace Path
    {
        /**
         * @brief Метод для соединения 2 путей.
         * @param l Левая часть пути.
         * @param r Правая часть пути.
         * @return Результат конкатенирования.
         */
        std::string join(const std::string &l, const std::string &r);

        /**
         * @brief Метод для проверки наличия файла.
         * @param p Путь к файлу.
         * @return Есть ли файл?
         */
        bool fileExists(const std::string &p);
    }

};


#endif //CARDMASTER_SYSTEMTOOLS_H
