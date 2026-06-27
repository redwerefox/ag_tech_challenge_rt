@echo off
setlocal

echo [0] Initializing MSVC x64 environment...

call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" ^
    -arch=x64 -host_arch=x64

if errorlevel 1 (
    echo Failed to initialize MSVC environment
    exit /b 1
)

echo [1] Checking compiler...
where cl
if errorlevel 1 (
    echo cl.exe not found after MSVC init
    exit /b 1
)

echo [1] Installing dependencies...
conan install . --build=missing --output-folder=build --profile:build=profiles/windows-msvc --profile:host=profiles/windows-msvc

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