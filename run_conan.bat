@echo off
setlocal

echo [1] Installing dependencies...
conan install . --build=missing --profile:build=profiles/windows-msvc --profile:host=profiles/windows-msvc

if %errorlevel% neq 0 exit /b %errorlevel%

echo [3] Configuring and Building...
cmake --preset conan-release
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build --preset conan-release
if %errorlevel% neq 0 exit /b %errorlevel%

echo [5] Running Tests...
ctest --preset conan-release
if %errorlevel% neq 0 exit /b %errorlevel%

echo Done!