
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

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

    PluginLoader(PluginLoader &&other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }

    PluginLoader &operator=(PluginLoader &&other) noexcept {
        if (this != &other) {
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

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

std::string find_plugin(const char *argv0) {
    auto exe_dir = std::filesystem::path(argv0).parent_path();
    if (exe_dir.empty()) {
        exe_dir = ".";
    }

#ifdef _WIN32
    constexpr const char *name = "plugin.dll";
#elif defined(__APPLE__)
    constexpr const char *name = "libplugin.dylib";
#else
    constexpr const char *name = "libplugin.so";
#endif

    // Build layout: plugin sits next to the executable.
    // Install layout (Linux/macOS): plugin lives in ../lib/ relative to bin/.
    for (auto &&candidate : {
             exe_dir / name,
             exe_dir / ".." / "lib" / name,
         }) {
        if (std::filesystem::exists(candidate)) {
            return std::filesystem::canonical(candidate).string();
        }
    }

    return (exe_dir / name).string();
}
} // namespace

int main(int /* argc */, char *argv[]) {
    auto plugin_path = find_plugin(*argv);
    std::cout << "Loading plugin from: " << plugin_path << "\n";

    PluginLoader loader(plugin_path);
    if (!loader.is_loaded()) {
        std::cerr << "Could not load the plugin library." << "\n";
        return EXIT_FAILURE;
    }

    auto init_fn = loader.get_symbol<plugin_init_fn>("plugin_init");
    auto name_fn = loader.get_symbol<plugin_get_name_fn>("plugin_get_name");
    auto add_fn = loader.get_symbol<plugin_add_fn>("plugin_add");

    if (init_fn == nullptr || name_fn == nullptr || add_fn == nullptr) {
        std::cerr << "Failed to resolve plugin symbols." << "\n";
        return EXIT_FAILURE;
    }

    int return_code = init_fn();
    if (return_code != 0) {
        std::cerr << "plugin_init failed with code " << return_code << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "Plugin name : " << name_fn() << "\n";
    std::cout << "plugin_add(3, 4) = " << add_fn(3, 4) << "\n";

    return EXIT_SUCCESS;
}
