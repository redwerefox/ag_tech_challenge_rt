
# Exit immediately if any command fails (Crucial for CI/CD!)
set -e

conan install . --build=missing -s build_type=Release --profile:build=profiles/linux-clang --profile:host=profiles/linux-clang
cmake --preset conan-release -DENABLE_FORMAT_CHECK=ON
cmake --build --preset conan-release --target format-check
ctest --preset conan-release
