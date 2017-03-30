//
// Created by megaxela on 08.12.16.
//

#ifndef FRTESTER_IPV4ADDRESS_H
#define FRTESTER_IPV4ADDRESS_H

#include "Tools/Platform.h"
#include <string>
#ifdef OS_LINUX
    #include <netinet/in.h>
#endif
#ifdef OS_WINDOWS
    typedef uint32_t in_addr_t;
#endif

/**
 * @brief IPv4 network address.
 */
class IPv4Address
{
public:
    /**
     * @brief Default constructor, builds 127.0.0.1 address.
     */
    IPv4Address();

    /**
     * @brief Destructor.
    */
    ~IPv4Address();

    /**
     * @brief Method for taking string implementation of address.
     */
    std::string toString() const;

    /**
     * @brief Method to get address value.
     */
    in_addr_t data() const;

    /**
     * @brief Method for setting address value from string.
     * @param s
     */
    bool setFromString(const std::string &s);

    /**
     * @brief Method for setting address value from c value.
     * @param value
     */
    void setData(const in_addr_t &value);

    /**
     * @brief Method for creating address value from string.
     * @param s String representation.
     * @return Address instance.
     */
    static IPv4Address fromString(const std::string &s);

private:
    in_addr_t m_data;
};



#endif //FRTESTER_IPV4ADDRESS_H
