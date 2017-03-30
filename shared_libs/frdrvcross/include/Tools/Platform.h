//
// Created by Megaxela on 22.03.2016.
//

#ifndef MIDRVCROSS_PLATFORM_H
#define MIDRVCROSS_PLATFORM_H

// OS_LINUX OS_WINDOWS
#if defined(__linux__) or\
    defined(__linux) or\
    defined(__gnu_linux__) or\
    defined(linux)
#define OS_LINUX
#else
#define OS_WINDOWS
#endif

#endif //MIDRVCROSS_PLATFORM_H
