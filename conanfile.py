from conan import ConanFile
from conan.tools.cmake import cmake_layout

class ChallengeConan(ConanFile):
    name = "challenge"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    default_options = {
        "boost/*:shared": True,
        "gtest/*:shared": True,
    }

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        # require boost log library
        self.requires("boost/1.91.0")
        self.requires("gtest/1.17.0")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.21]")
        self.tool_requires("ninja/[>=1.10]")
