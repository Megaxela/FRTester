//
// Created by megaxela on 08.12.16.
//

#include <include/Tools/Logger.h>
#include <sstream>
#include "include/Tools/IPv4Address.h"

IPv4Address::IPv4Address() :
    m_data((in_addr_t) 0x7f000001)
{

}

IPv4Address::~IPv4Address()
{

}

std::string IPv4Address::toString() const
{
    std::stringstream ss;
    unsigned char bytes[4];
    bytes[0] = (uint8_t) ( m_data & 0xFF);
    bytes[1] = (uint8_t) ((m_data >> 8) & 0xFF);
    bytes[2] = (uint8_t) ((m_data >> 16) & 0xFF);
    bytes[3] = (uint8_t) ((m_data >> 24) & 0xFF);

    ss << bytes[3] << '.' << bytes[2] << '.' << bytes[1] << bytes[0];
    return ss.str();
}


in_addr_t IPv4Address::data() const
{
    return m_data;
}

bool IPv4Address::setFromString(const std::string &s)
{
    in_addr_t type = 0x00000000;

    uint8_t inc = 0;
//    uint8_t level = 0;
    uint8_t byteN = 0;

    for (auto ch : s)
    {
        if (ch == '.')
        {

            type |= inc << ((byteN++) * 8);

            inc = 0;
        }
        else if (ch >= '0' && ch <= '9')
        {
            // If overflow
            if (inc > 25 || (inc == 25 && ch > '5'))
            {
                Error("Can't parse '" + s + "'. Wrong format.");
                return false;
            }

            inc = inc * static_cast<uint8_t>(10) + (ch - '0');
        }
        else
        {
            Error("Can't parse '" + s + "'. Wrong format.");
            return false;
        }
    }

    type |= inc << ((byteN++) * 8);

    if (byteN != 4)
    {
        Error("Can't parse '" + s + "'. Wrong format.");
        return false;
    }

    m_data = type;

    return true;
}

void IPv4Address::setData(const in_addr_t &value)
{
    m_data = value;
}

IPv4Address IPv4Address::fromString(const std::string &s)
{
    IPv4Address address;

    address.setFromString(s);

    return address;
}
