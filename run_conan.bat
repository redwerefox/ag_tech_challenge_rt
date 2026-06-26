source venv-conan\Scripts\activate
conan install . --build=missing --profile profiles/windows-msvc
call build\Release\generators\conanbuild.bat
cmake --preset conan-release
cmake --build --preset conan-release
call build\Release\generators\deactivate_conanbuild.bat
call build\Release\generators\conanrun.bat
ctest --preset conan-release