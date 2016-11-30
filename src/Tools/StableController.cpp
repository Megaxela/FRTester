//
// Created by megaxela on 21.11.16.
//


#include <csignal>
#include <stdlib.h>
#include "StableController.h"

void posix_death_signal(int signum)
{
    StableController::excessExecute();

    signal(signum, SIG_DFL);
    exit(-3);
}

void StableController::init(std::function<void()> cb)
{
    m_cb = cb;

    signal(SIGSEGV, posix_death_signal);
}

void StableController::excessExecute()
{
    m_cb();
}
