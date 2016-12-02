//
// Created by megaxela on 28.11.16.
//

#include "include/Testing/TestEnvironment.h"

TestEnvironment::TestEnvironment(
        TestDriver *driver,
        TestLogger* logger
) :
    m_driver(driver),
    m_logger(logger)
{

}

TestDriver *TestEnvironment::driver() const
{
    return m_driver;
}

TestLogger *TestEnvironment::logger() const
{
    return m_logger;
}
