//
// Created by megaxela on 10.01.17.
//

#ifndef FRTESTER_FROPERATIONS_H
#define FRTESTER_FROPERATIONS_H


#include <cstdint>

namespace FROperations
{
    /**
     * @brief Функция, выполняющая округление по правилам
     * для ФР.
     * @param val Значение для округления.
     * @return Результат округления.
     */
    uint32_t smartRound(double val);
}


#endif //FRTESTER_FROPERATIONS_H
