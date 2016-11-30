//
// Created by megaxela on 22.11.16.
//

#include <include/Testing/TestEnvironment.h>
#include "include/Testing/AbstractTest.h"

AbstractTest::AbstractTest(TestEnvironment *driver,
                           const std::string &name,
                           const std::string &description) :
    m_enviroment(driver),
    m_name(name),
    m_description(description)
{

}

AbstractTest::~AbstractTest()
{

}

TestEnvironment *AbstractTest::enviroment()
{
    return m_enviroment;
}

std::string AbstractTest::name() const
{
    return m_name;
}

std::string AbstractTest::description() const
{
    return m_description;
}

void AbstractTest::setName(const std::string &name)
{
    m_name = name;
}

void AbstractTest::setDescription(const std::string &description)
{

}
