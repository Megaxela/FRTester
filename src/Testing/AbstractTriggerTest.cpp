//
// Created by megaxela on 22.11.16.
//

#include <Tools/Logger.h>
#include <include/Testing/TestEnvironment.h>
#include "include/Testing/AbstractTriggerTest.h"

AbstractTriggerTest::AbstractTriggerTest(TestEnvironment *environment, const std::string &name,
                                         const std::string &description, bool critical) :
    m_name(name),
    m_description(description),
    m_critical(critical),
    m_environment(environment)
{

}

AbstractTriggerTest::~AbstractTriggerTest()
{

}

std::string AbstractTriggerTest::name() const
{
    return m_name;
}

std::string AbstractTriggerTest::description() const
{
    return m_description;
}

void AbstractTriggerTest::setName(const std::string &name)
{
    m_name = name;
}

void AbstractTriggerTest::setDescription(const std::string &description)
{
    m_description = description;
}

bool AbstractTriggerTest::isCritical() const
{
    return m_critical;
}

TestEnvironment *AbstractTriggerTest::environment() const
{
    return m_environment;
}
