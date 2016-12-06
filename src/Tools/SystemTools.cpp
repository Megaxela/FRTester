//
// Created by Megaxela on 18.10.2016.
//

#include "Tools/SystemTools.h"
#include "include/Tools/Logger.h"

#ifdef OS_LINUX
    #include <dirent.h>
#endif
#ifdef OS_WINDOWS
    #include "libraries/dirent/dirent.h"
#endif

std::vector<std::string> SystemTools::getAllFilesInDir(const std::string &path)
{
    std::vector<std::string> result;

    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string tmp = std::string(ent->d_name);
            if (tmp == "." || tmp == "..")
            {
                continue;
            }

            result.push_back(tmp);
        }
    }
    else
    {
        Critical("Такой папки не существует.");
    }

    return result;
}

#ifdef OS_WINDOWS
    #define PATH_SEPARATOR ('\\')
#endif
#ifdef OS_LINUX
    #define PATH_SEPARATOR ('/')
#endif

std::string SystemTools::Path::join(const std::string &l, const std::string &r)
{
    if (l[l.length() - 1] != PATH_SEPARATOR)
    {
        return (l + PATH_SEPARATOR + r);
    }
    else
    {
        return (l + r);
    }
}