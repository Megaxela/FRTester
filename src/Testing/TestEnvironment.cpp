//
// Created by megaxela on 28.11.16.
//

#include "include/Testing/TestEnvironment.h"

TestEnvironment::TestEnvironment(TestDriver *driver) :
    m_driver(driver)
{

}

TestDriver *TestEnvironment::driver() const
{
    return m_driver;
}
