#include <gtest/gtest.h>
#include <filesystem>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using crash_fn = void (*)();

static constexpr const char *LOG_PATH = CRASH_LOG_PATH;
namespace {

void *load_lib(const char *path) {
#ifdef _WIN32
    auto lib = LoadLibraryA(path);
    return lib;
#else
    auto *lib = dlopen(path, RTLD_NOW);

    return lib;
#endif
}

void *get_sym(void *lib, const char *name) {
#ifdef _WIN32
    return (void *)GetProcAddress((HMODULE)lib, name);
#else
    return dlsym(lib, name);
#endif
}

class LoadPluginAndSegfaultDetect : public ::testing::Test {
  protected:
    void SetUp() override {
        const char *det_lib_path_ = std::getenv("DETECTOR_PATH"); // NOLINT(concurrency-mt-unsafe) - out of time
        const char *plug_lib_path_ = std::getenv("PLUGIN_PATH");  // NOLINT(concurrency-mt-unsafe) - out of time
        det_lib_ = load_lib(det_lib_path_);
        plug_lib_ = load_lib(plug_lib_path_);
    }

    void TearDown() override {
        if (det_lib_ != nullptr) {
#ifdef _WIN32
            FreeLibrary((HMODULE)det_lib_);
#else
            dlclose(det_lib_);
#endif
        }
        if (plug_lib_ != nullptr) {
#ifdef _WIN32
            FreeLibrary((HMODULE)plug_lib_);
#else
            dlclose(plug_lib_);
#endif
        }
    }

    void *det_lib_ = nullptr;
    void *plug_lib_ = nullptr;
};
} // namespace

TEST_F(LoadPluginAndSegfaultDetect, PluginLoadsSuccessfully) { EXPECT_NE(plug_lib_, nullptr); }

TEST_F(LoadPluginAndSegfaultDetect, SegfaultDetectorLoadsSuccessfully) { EXPECT_NE(det_lib_, nullptr); }

TEST_F(LoadPluginAndSegfaultDetect, PluginSegfaultsAreDetected) {
    ASSERT_NE(det_lib_, nullptr);
    ASSERT_NE(plug_lib_, nullptr);

    auto detector_init = (void (*)())get_sym(det_lib_, "detector_init");         // NOLINT - out of time
    auto detector_shutdown = (void (*)())get_sym(det_lib_, "detector_shutdown"); // NOLINT - out of time
    ASSERT_NE(detector_init, nullptr);
    ASSERT_NE(detector_shutdown, nullptr);

    // Initialize the detector
    detector_init();

    auto plugin_crash_segfault = (crash_fn)get_sym(plug_lib_, "plugin_crash_segfault"); // NOLINT - out of time
    ASSERT_NE(plugin_crash_segfault, nullptr);

    // Call the function that will cause a segfault inside a Death Test.
    // The test will PASS if the code crashes.
    ASSERT_DEATH({ plugin_crash_segfault(); }, ".*"); // Optional: replace ".*" with expected output from your detector

    detector_shutdown();
}

// Google Test recommends death test fixtures end in "DeathTest"
using LoadPluginAndSegfaultDetectDeathTest = LoadPluginAndSegfaultDetect;

TEST_F(LoadPluginAndSegfaultDetectDeathTest, PluginWritesStackTraceOnSegfault) {

    std::cerr << "CRASH_LOG_PATH = [" << CRASH_LOG_PATH << "]\n";

    ASSERT_NE(det_lib_, nullptr);
    ASSERT_NE(plug_lib_, nullptr);

    auto detector_init = (void (*)())get_sym(det_lib_, "detector_init");         // NOLINT - out of time
    auto detector_shutdown = (void (*)())get_sym(det_lib_, "detector_shutdown"); // NOLINT - out of time
    ASSERT_NE(detector_init, nullptr);
    ASSERT_NE(detector_shutdown, nullptr);

    // Initialize the detector
    detector_init();

    auto plugin_crash_segfault = (crash_fn)get_sym(plug_lib_, "plugin_crash_segfault"); // NOLINT - out of time
    ASSERT_NE(plugin_crash_segfault, nullptr);

    // ASSERT_DEATH will run the code in a separate process, so we can't check for the file existence in the same
    // process. Instead, we can check for the file existence after the death test has run.
    ASSERT_DEATH({ plugin_crash_segfault(); }, ".*"); // Optional:

    // check for file

    // Inside your test:
    namespace fs = std::filesystem;
    fs::path backtrace_file = CRASH_LOG_PATH; // Use the macro here!

    EXPECT_TRUE(fs::exists(backtrace_file)) << "Expected " << backtrace_file << " to be created after segfault.";
    ASSERT_TRUE(std::filesystem::exists(backtrace_file))
        << "Expected backtrace.dump file to be created after segfault."; // Clean up after test

    detector_shutdown();
}