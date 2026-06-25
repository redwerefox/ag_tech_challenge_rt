from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class ChallengeConan(ConanFile):
    name = "challenge"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    default_options = {
        "boost:shared": True,
        "gtest:shared": True,
    }

    def requirements(self):
        # require boost log library
        self.requires("boost/1.82.0")

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

