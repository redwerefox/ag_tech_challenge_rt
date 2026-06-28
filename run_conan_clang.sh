
# Exit immediately if any command fails (Crucial for CI/CD!)
set -e

conan install . --build=missing -s build_type=Release --output-folder=build --profile:build=profiles/linux-clang --profile:host=profiles/linux-clang
cmake --preset conan-release
cmake --build --preset conan-release
ctest --preset conan-release
