//
// Created by megaxela on 11.11.16.
//

#include "Abstract/Protocol.h"

Protocol::Protocol() :
    m_smallTimeoutMcs(5 * 1000 * 1000),
    m_bigTimeoutMcs(2 * 60 * 1000 * 1000)
{

}

Protocol::~Protocol()
{

}

uint32_t Protocol::smallTimeoutMcs()
{
    return m_smallTimeoutMcs;
}

uint32_t Protocol::bigTimeoutMcs()
{
    return m_bigTimeoutMcs;
}
