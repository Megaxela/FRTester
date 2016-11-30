//
// Created by Megaxela on 18.10.2016.
//

#ifndef CARDMASTER_SYSTEMTOOLS_H
#define CARDMASTER_SYSTEMTOOLS_H

#include <cinttypes>

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
};


#endif //CARDMASTER_SYSTEMTOOLS_H
