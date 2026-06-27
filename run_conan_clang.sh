
conan install . --build=missing -s build_type=Release -profile=linux-clang
source build/Release/generators/conanbuild.sh
cmake --preset conan-release
cmake --build --preset conan-release
source build/Release/generators/deactivate_conanbuildenv.sh
source build/Release/generators/conanrelease.sh
ctest --preset conan-release
