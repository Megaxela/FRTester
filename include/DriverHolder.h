//
// Created by megaxela on 17.11.16.
//

#ifndef FRTESTER_DRIVERHOLDER_H
#define FRTESTER_DRIVERHOLDER_H

#include "FRDriver.h"

/**
 * @brief Типа синглетон, хранящий объект драйвера, чтобы
 * он был доступен из любого места в программе.
 */
namespace DriverHolder
{
    FRDriver& driver();
};


#endif //FRTESTER_DRIVERHOLDER_H
