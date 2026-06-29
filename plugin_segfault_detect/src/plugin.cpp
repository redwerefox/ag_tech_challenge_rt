#include "plugin/plugin.h"

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

#include <execinfo.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#include <csignal>
#include <boost/log/trivial.hpp>
#include <boost/stacktrace.hpp>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

static constexpr const char* LOG_PATH = CRASH_LOG_PATH;

static void segv_handler(int signum)
{
      constexpr int MAX_FRAMES = 64;
    void* frames[MAX_FRAMES];
    int n = backtrace(frames, MAX_FRAMES);

    int fd = open(CRASH_LOG_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        // First line: signal info
        const char header[] = "=== SEGFAULT ===\n";
        write(fd, header, sizeof(header) - 1);
        // Dump raw stack (async-signal-safe, prints addresses + nearest symbol)
        backtrace_symbols_fd(frames, n, fd);
        close(fd);
    }

    std::signal(signum, SIG_DFL);
    std::raise(signum);



}

void detector_init()
{
#ifdef _WIN32
    AddVectoredExceptionHandler(1,
                                [](EXCEPTION_POINTERS *) -> LONG
                                {
                                    segv_handler(SIGSEGV);
                                    return EXCEPTION_CONTINUE_SEARCH;
                                });
#else
    struct sigaction sa{};
    sa.sa_handler = [](int)
    {
        segv_handler(SIGSEGV);
    };

    sigaction(SIGSEGV, &sa, nullptr);
#endif

    BOOST_LOG_TRIVIAL(debug) << "[detector] initialized";
}

void detector_shutdown()
{
    BOOST_LOG_TRIVIAL(debug) << "[detector] shutdown";
}