#include "plugin/plugin.h"

#include <fstream>
#include <csignal>
#include <boost/log/trivial.hpp>
#include <boost/stacktrace.hpp>

#ifdef _WIN32
#include <windows.h>
static PVOID g_vehHandle = nullptr;
#endif

// Shared minimal writer
static void dump_stacktrace()
{
    std::ofstream(CRASH_LOG_PATH) << boost::stacktrace::stacktrace();
}

#ifdef _WIN32
static LONG WINAPI crash_handler(EXCEPTION_POINTERS *)
{
    dump_stacktrace();
    return EXCEPTION_CONTINUE_SEARCH; // Required so GTest can process the failure
}
#else
static void crash_handler(int signum)
{
    dump_stacktrace();
    std::signal(signum, SIG_DFL);
    std::raise(signum);
}
#endif

void detector_init()
{
#ifdef _WIN32
    g_vehHandle = AddVectoredExceptionHandler(1, crash_handler);
#else
    std::signal(SIGSEGV, crash_handler);
#endif

    BOOST_LOG_TRIVIAL(debug) << "[detector] initialized";
}

void detector_shutdown()
{
#ifdef _WIN32
    if (g_vehHandle)
        RemoveVectoredExceptionHandler(g_vehHandle);
#else
    std::signal(SIGSEGV, SIG_DFL);
#endif

    BOOST_LOG_TRIVIAL(debug) << "[detector] shutdown";
}