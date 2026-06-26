conan install . --build=missing --profile profiles/windows-msvc

cmake --preset conan-release
cmake --build --preset conan-release

ctest --preset conan-release