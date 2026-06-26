
conan install . --build=missing -s build_type=Release -profile=linux-clang
cmake --preset conan-release
cmake --build --preset conan-release
ctest --preset conan-release
