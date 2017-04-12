//
// Created by megaxela on 15.12.16.
//

#include "Tools/StdExtend.h"

STDEXTEND_NAMESPACE
{
    bool begins(const std::string &s, const std::string &with)
    {
        return s.compare(0, with.size(), with) == 0;
    }

    bool ends(const std::string &s, const std::string &with)
    {
        return s.compare(s.size() - with.size(), s.size(), with) == 0;
    }

    std::vector<std::string> split(const std::string &s, char delim)
    {
        std::vector<std::string> internal;
        std::stringstream ss(s); // Turn the string into a stream.
        std::string tok;

        while(getline(ss, tok, delim)) {
            internal.push_back(tok);
        }

        return internal;
    }
}