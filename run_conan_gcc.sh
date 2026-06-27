
# Exit immediately if any command fails (Crucial for CI/CD!)
set -e

echo "[1] conan install..."
conan install . --build=missing -s build_type=Release --profile:build=profiles/ubuntu-gcc --profile:host=profiles/ubuntu-gcc
echo "[2] Activating Conan build environment..."
source build/Release/generators/conanbuild.sh
echo "[3] Configuring and Building..."
cmake --preset conan-release
cmake --build --preset conan-release
echo "[4] Deactivating Conan build environment..."
source build/Release/generators/deactivate_conanbuild.sh
echo "[5] Activating Conan release environment..."
source build/Release/generators/conanrelease.sh
echo "[6] Running tests..."
ctest --preset conan-release
