//
// Created by megaxela on 21.11.16.
//


#include <csignal>
#include <stdlib.h>
#include "Tools/StableController.h"
#include <Tools/Platform.h>

void posix_death_signal(int signum)
{
    StableController::excessExecute();

    signal(signum, SIG_DFL);
    exit(-3);
}

void StableController::init(std::function<void()> cb)
{
    _cb = cb;

    signal(SIGSEGV, posix_death_signal);
    signal(SIGFPE, posix_death_signal);
    signal(SIGABRT, posix_death_signal);

#ifdef OS_LINUX
    signal(SIGKILL, posix_death_signal);
#endif
}

void StableController::excessExecute()
{
    _cb();
}
