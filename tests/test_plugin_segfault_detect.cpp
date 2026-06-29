#include <gtest/gtest.h>

#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using crash_fn = void (*)();

static void *load_lib(const char *path)
{
#ifdef _WIN32
    return LoadLibraryA(path);
#else
    return dlopen(path, RTLD_NOW);
#endif
}

static void *get_sym(void *lib, const char *name)
{
#ifdef _WIN32
    return (void *)GetProcAddress((HMODULE)lib, name);
#else
    return dlsym(lib, name);
#endif
}

class LoadPluginAndSegfaultDetect : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const char *det_lib_path_ = std::getenv("DETECTOR_PATH");
        const char *plug_lib_path_ = std::getenv("PLUGIN_PATH");
        std::cerr << "PLUGIN_PATH = "
                  << (plug_lib_path_ ? plug_lib_path_ : "NULL") << "\n";
        std::cerr << "DETECTOR_PATH = "
                  << (det_lib_path_ ? det_lib_path_ : "NULL") << "\n";
        det_lib_ = load_lib(det_lib_path_);
        plug_lib_ = load_lib(plug_lib_path_);
    }

    void TearDown() override
    {
        if (det_lib_)
        {
#ifdef _WIN32
            FreeLibrary((HMODULE)det_lib_);
#else
            dlclose(det_lib_);
#endif
        }
        if (plug_lib_)
        {
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

TEST_F(LoadPluginAndSegfaultDetect, PluginLoadsSuccessfully)
{
    EXPECT_NE(plug_lib_, nullptr);
}

TEST_F(LoadPluginAndSegfaultDetect, SegfaultDetectorLoadsSuccessfully)
{
    EXPECT_NE(det_lib_, nullptr);
}

TEST_F(LoadPluginAndSegfaultDetect, PluginSegfaultsAreDetected)
{
    ASSERT_NE(det_lib_, nullptr);
    ASSERT_NE(plug_lib_, nullptr);

    auto detector_init = (void (*)())get_sym(det_lib_, "detector_init");
    ASSERT_NE(detector_init, nullptr);

    // Initialize the detector
    detector_init();

    auto plugin_crash_segfault = (crash_fn)get_sym(plug_lib_, "plugin_crash_segfault");
    ASSERT_NE(plugin_crash_segfault, nullptr);

    // Call the function that will cause a segfault inside a Death Test.
    // The test will PASS if the code crashes.
    ASSERT_DEATH({ plugin_crash_segfault(); }, ".*"); // Optional: replace ".*" with expected output from your detector
}