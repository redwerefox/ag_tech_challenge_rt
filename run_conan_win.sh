conan install . --build=missing --profile profiles/windows-msvc
cmake --preset conan-default
cmake --build --preset conan-release
ctest --preset conan-release