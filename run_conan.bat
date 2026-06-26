conan install . --build=missing --profile profiles/windows-msvc
build/Release/generators/conanbuildenv.bat
cmake --preset conan-release
cmake --build --preset conan-release
build/Release/generators/deactivate_conanbuildenv.bat
build/Release/generators/conanrelease.bat
ctest --preset conan-release