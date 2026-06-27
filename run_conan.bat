@echo off

echo [1] Installing dependencies...
conan install . --build=missing --profile profiles/windows-msvc
if %errorlevel% neq 0 exit /b %errorlevel%

echo [2] Activating Conan build environment...
call build\Release\generators\conanbuild.bat


echo [3] Configuring and Building...
cmake --preset conan-release
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build --preset conan-release
if %errorlevel% neq 0 exit /b %errorlevel%

echo [4] Switching to Runtime environment...
call build\Release\generators\deactivate_conanbuild.bat
call build\Release\generators\conanrun.bat

echo [5] Running Tests...
ctest --preset conan-release
if %errorlevel% neq 0 exit /b %errorlevel%

echo Done!