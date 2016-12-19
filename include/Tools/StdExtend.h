//
// Created by megaxela on 15.12.16.
//

#ifndef FRTESTER_STDEXTEND_H
#define FRTESTER_STDEXTEND_H

//#define STD_NAMESPACE
#include <string>
#include <sstream>

#ifdef STD_NAMESPACE
    #define STDEXTEND_NAMESPACE namespace std
#else
    #define STDEXTEND_NAMESPACE namespace stdex
#endif

STDEXTEND_NAMESPACE
{
    /**
     * @brief Serialisation to string for all supported types.
     * @tparam T
     * @param val
     * @return
     */
    template<typename T>
    std::string to_string(T val)
    {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }

    /**
     * @brief Method for checking is string begins with...
     * @param s String.
     * @param with String begin.
     * @return Is string `s` begins with string `with`
     */
    bool begins(const std::string &s, const std::string &with);
};


#endif //FRTESTER_STDEXTEND_H
