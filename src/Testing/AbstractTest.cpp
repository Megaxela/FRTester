//
// Created by megaxela on 22.11.16.
//

#include "AbstractTest.h"

AbstractTest::AbstractTest(TestDriver *driver) :
    m_driver(driver)
{

}

AbstractTest::~AbstractTest()
{

}

TestDriver *AbstractTest::driver()
{
    return m_driver;
}
