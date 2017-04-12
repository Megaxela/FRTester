//
// Created by megaxela on 15.12.16.
//

#ifndef FRTESTER_STDEXTEND_H
#define FRTESTER_STDEXTEND_H

//#define STD_NAMESPACE
#include <string>
#include <sstream>
#include <vector>

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

    /**
     * @brief Method for checking is string ends with...
     * @param s String.
     * @param with String end.
     * @return Is string `s` ends with string `with`
     */
    bool ends(const std::string &s, const std::string &with);

    /**
     * @brief Method for getting median value in container.
     * @tparam T Int type.
     * @tparam Container
     * @param container
     * @return
     */
    template<typename Container>
    typename Container::value_type median(const Container &container)
    {
        if (container.size() == 0)
        {
            return 0;
        }

        typename Container::value_type sum = typename Container::value_type(0);
        for (auto& el : container)
        {
            sum += el;
        }

        return sum / container.size();
    }

    /**
     * @brief Method for splitting string with delimiter.
     * @param s String to be splitted.
     * @param delim Delimiter.
     * @return Splitted string.
     */
    std::vector<std::string> split(const std::string& s, char delim);
}


#endif //FRTESTER_STDEXTEND_H
