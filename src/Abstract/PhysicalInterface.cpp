//
// Created by megaxela on 13.11.16.
//

#include "Abstract/PhysicalInterface.h"

PhysicalInterface::PhysicalInterface(int t) :
    m_t(t)
{

}

PhysicalInterface::~PhysicalInterface()
{

}

int PhysicalInterface::type() const
{
    return m_t;
}
