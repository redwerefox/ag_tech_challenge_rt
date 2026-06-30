#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <string>

#include <mutex>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace {

using plugin_init_fn = int (*)();
using plugin_get_name_fn = const char *(*)();
using plugin_add_fn = int (*)(int, int);

namespace {
class PluginLoader {
  public:
    explicit PluginLoader(const std::string &path)
        :
#ifdef _WIN32
          handle_(LoadLibraryA(path.c_str())) {
#else
          handle_(dlopen(path.c_str(), RTLD_LAZY)) {
#endif
    }

    ~PluginLoader() {
        if (handle_ == nullptr) {
            return;
        }
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(handle_));
#else
        dlclose(handle_);
#endif
    }

    PluginLoader(const PluginLoader &) = delete;
    PluginLoader &operator=(const PluginLoader &) = delete;
    PluginLoader(PluginLoader &&other) = delete;
    PluginLoader &operator=(PluginLoader &&other) = delete;

    template <typename T> T get_symbol(const char *name) const {
        if (handle_ == nullptr) {
            return nullptr;
        }
#ifdef _WIN32
        return reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(handle_), name));
#else
        return reinterpret_cast<T>(dlsym(handle_, name)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
#endif
    }

    [[nodiscard]] bool is_loaded() const { return handle_ != nullptr; }

  private:
    void *handle_ = nullptr;
};

std::string get_plugin_path() {

    char *env = nullptr;
    {
        static std::mutex env_mutex;
        std::lock_guard<std::mutex> lock(env_mutex);
        env = std::getenv(  // NOLINT(concurrency-mt-unsafe) - getenv is not thread-safe, but we are locking around it
            "PLUGIN_PATH"); // NOLINT(concurrency-mt-unsafe) -
    }
    if (env != nullptr) {
        return env;
    }

    auto exe_dir = std::filesystem::path(".");
#ifdef _WIN32
    return (exe_dir / "plugin.dll").string();
#elif defined(__APPLE__)
    return (exe_dir / "libplugin.dylib").string();
#else
    return (exe_dir / "libplugin.so").string();
#endif
}

class PluginTest : public ::testing::Test {
  protected:
    void SetUp() override {
        loader_ = std::make_unique<PluginLoader>(get_plugin_path());
        ASSERT_TRUE(loader_->is_loaded()) << "Plugin library not found. "
                                             "Set PLUGIN_PATH or run from the build/bin directory.";
    }

    void TearDown() override { loader_.reset(); }

    std::unique_ptr<PluginLoader> loader_;
};
} // namespace

TEST_F(PluginTest, PluginLoadsSuccessfully) { EXPECT_TRUE(loader_->is_loaded()); }

TEST_F(PluginTest, InitReturnsZero) {
    auto fn = loader_->get_symbol<plugin_init_fn>("plugin_init"); // NOLINT out of time
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(), 0);
}

TEST_F(PluginTest, GetNameReturnsExpected) {
    auto fn = loader_->get_symbol<plugin_get_name_fn>("plugin_get_name"); // NOLINT out of time
    ASSERT_NE(fn, nullptr);
    EXPECT_STREQ(fn(), "challange_plugin");
}

TEST_F(PluginTest, AddReturnsCorrectSum) {
    auto fn = loader_->get_symbol<plugin_add_fn>("plugin_add"); // NOLINT out of time
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(2, 3), 5);
    EXPECT_EQ(fn(-1, 1), 0);
    EXPECT_EQ(fn(0, 0), 0);
    EXPECT_EQ(fn(100, 200), 300);
}

TEST_F(PluginTest, AllSymbolsResolvable) {
    EXPECT_NE(loader_->get_symbol<plugin_init_fn>("plugin_init"), nullptr);
    EXPECT_NE(loader_->get_symbol<plugin_get_name_fn>("plugin_get_name"), nullptr);
    EXPECT_NE(loader_->get_symbol<plugin_add_fn>("plugin_add"), nullptr);
}

} // namespace
