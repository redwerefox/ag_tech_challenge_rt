
# Exit immediately if any command fails (Crucial for CI/CD!)
set -e

echo "[1] conan install..."
conan install . --build=missing -s build_type=Release --profile:build=profiles/linux-gcc --profile:host=profiles/linux-gcc
echo "[2] Activating Conan build environment..."
source build/Release/generators/conanbuild.sh
echo "[3] Configuring and Building..."
cmake --preset conan-release
cmake --build --preset conan-release
echo "[5] Activating Conan release environment..."
source build/Release/generators/conanrun.sh
echo "[6] Running tests..."
ctest --preset conan-release
