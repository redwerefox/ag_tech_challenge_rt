#include "plugin/plugin.h"

#include <fstream>
#include <csignal>
#include <boost/log/trivial.hpp>
#include <boost/stacktrace.hpp>
#include <tuple>

#ifdef _WIN32
#include <windows.h>
static PVOID g_vehHandle = nullptr;
#endif

// Shared minimal writer
namespace {
void dump_stacktrace() { std::ofstream(CRASH_LOG_PATH) << boost::stacktrace::stacktrace(); }

#ifdef _WIN32
static LONG WINAPI crash_handler(EXCEPTION_POINTERS *) {
    dump_stacktrace();
    return EXCEPTION_CONTINUE_SEARCH; // Required so GTest can process the failure
}
#else
void crash_handler(int signum) {
    dump_stacktrace();
    std::ignore = std::signal(signum, SIG_DFL);
    std::ignore = std::raise(signum);
}
#endif
} // namespace

extern "C" {

DETECTOR_API void detector_init() {
#ifdef _WIN32
    g_vehHandle = AddVectoredExceptionHandler(1, crash_handler);
#else
    std::ignore = std::signal(SIGSEGV, crash_handler);
#endif

    BOOST_LOG_TRIVIAL(debug) << "[detector] initialized";
}

DETECTOR_API void detector_shutdown() {
#ifdef _WIN32
    if (g_vehHandle)
        RemoveVectoredExceptionHandler(g_vehHandle);
#else
    std::ignore = std::signal(SIGSEGV, SIG_DFL);
#endif

    BOOST_LOG_TRIVIAL(debug) << "[detector] shutdown";
}
}