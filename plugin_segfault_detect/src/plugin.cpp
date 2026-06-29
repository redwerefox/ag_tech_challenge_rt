#include "plugin/plugin.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

static void handler()
{
    std::cerr << "[detector] segfault caught\n";
}

void detector_init()
{
#ifdef _WIN32
    AddVectoredExceptionHandler(1,
                                [](EXCEPTION_POINTERS *) -> LONG
                                {
                                    std::cerr << "[detector] SEGV caught (VEH)\n";
                                    return EXCEPTION_CONTINUE_SEARCH;
                                });
#else
    struct sigaction sa{};
    sa.sa_handler = [](int)
    {
        std::cerr << "[detector] SEGV caught\n";
    };

    sigaction(SIGSEGV, &sa, nullptr);
#endif

    std::cerr << "[detector] initialized\n";
}

void detector_shutdown()
{
    std::cerr << "[detector] shutdown\n";
}