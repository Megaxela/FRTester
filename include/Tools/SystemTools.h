//
// Created by Megaxela on 18.10.2016.
//

#ifndef CARDMASTER_SYSTEMTOOLS_H
#define CARDMASTER_SYSTEMTOOLS_H

#include <cinttypes>
#include <vector>
#include <string>
#include <cxxabi.h>
#include <iostream>
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

    /**
     * @brief Возвращает количество полезных бит в числе.
     * @tparam T Тип числа.
     * @param number Число.
     * @return Количество бит.
     */
    template< typename T >
    uint16_t getNumberSize(T number)
    {
        uint16_t r = 0;
        T mask = shl((T) 1, sizeof(T) * 8 - 1);

        while (mask)
        {
            if (number & mask)
            {
                return (sizeof(T) * 8) - r;
            }

            ++r;
            mask >>= 1;
        }

        return (sizeof(T) * 8)- r;
    }


    template<typename T>
    std::string getTypeName(const T &t)
    {
        int status;
        char* demangled = abi::__cxa_demangle(typeid(t).name(),0,0,&status);

        std::string result;
        if (status == -1)
        {
            std::cerr << "Can't allocate memory." << std::endl;
            return result;
        }
        else if (status == -2)
        {
            std::cerr << "Wrong mangled name." << std::endl;
            return result;
        }
        else if (status == -3)
        {
            std::cerr << "Some argument is invalid." << std::endl;
            return result;
        }

        result = demangled;

        free(demangled);

        return result;
    }

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
