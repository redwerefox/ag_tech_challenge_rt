from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class ChallengeConan(ConanFile):
    name = "challenge"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # require boost log library
        self.requires("boost/1.82.0")
        #if linux, we require dlfcnf
        if self.settings.os == "Linux":
            self.requires("dlfcnf/1.0.0")

    def test_requirements(self):
        self.test_requires("gtest/1.13.0")

    def build_requirements(self):
        self.tool_requires("cmake/3.21.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

