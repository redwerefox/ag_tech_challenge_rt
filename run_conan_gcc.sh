
# Exit immediately if any command fails (Crucial for CI/CD!)
set -e

echo "[1] conan install..."
conan install . --build=missing -s build_type=Release --output-folder=build --profile:build=profiles/linux-gcc --profile:host=profiles/linux-gcc
cmake --preset conan-release
cmake --build --preset conan-release
ctest --preset conan-release
