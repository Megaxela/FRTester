//
// Created by megaxela on 28.11.16.
//

#include "include/Testing/TestEnvironment.h"

TestEnvironment::TestEnvironment(
        TestDriver *driver,
        TestLogger* logger,
        TestingTools* tools
) :
    m_driver(driver),
    m_logger(logger),
    m_tools(tools)
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

TestingTools *TestEnvironment::tools() const
{
    return m_tools;
}

