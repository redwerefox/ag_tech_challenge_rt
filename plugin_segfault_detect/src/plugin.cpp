#include "plugin/plugin.h"

#include <iostream>
#include <cstdlib>
#include <boost/stacktrace.hpp>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

static void write_stackTrace()
{
    std::cerr << "[detector] SEGV caught (VEH)\n";
    std::cerr << "--- Stacktrace ---\n"
              << boost::stacktrace::stacktrace()
              << "------------------\n";
}
static void handler()
{
    write_stackTrace();
}

void detector_init()
{
#ifdef _WIN32
    AddVectoredExceptionHandler(1,
                                [](EXCEPTION_POINTERS *) -> LONG
                                {
                                    write_stackTrace();
                                    return EXCEPTION_CONTINUE_SEARCH;
                                });
#else
    struct sigaction sa{};
    sa.sa_handler = [](int)
    {
        write_stackTrace();
    };

    sigaction(SIGSEGV, &sa, nullptr);
#endif

    std::cerr << "[detector] initialized\n";
}

void detector_shutdown()
{
    std::cerr << "[detector] shutdown\n";
}