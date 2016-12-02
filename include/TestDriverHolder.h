//
// Created by megaxela on 02.12.16.
//

#ifndef FRTESTER_TESTDRIVERHOLDER_H
#define FRTESTER_TESTDRIVERHOLDER_H

#include "Testing/TestDriver.h"

/**
 * @brief Типа синглетон, хранящий объект тестового
 * драйвера, чтобы он был доступен из любого места в программе.
 */
namespace TestDriverHolder
{
    TestDriver& driver();
};


#endif //FRTESTER_TESTDRIVERHOLDER_H
