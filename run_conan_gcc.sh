
conan install . --build=missing -s build_type=Release --profile=profiles/linux-gcc
cmake --preset conan-release
cmake --build --preset conan-release
ctest --preset conan-release
