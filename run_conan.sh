conan install . --build=missing -s build_type=Release
cmake --preset conan-release
cmake --build --preset conan-release
ctest --preset conan-release