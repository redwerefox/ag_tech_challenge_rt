from conan import ConanFile
from conan.tools.cmake import cmake_layout

class ChallengeConan(ConanFile):
    name = "challenge"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"


    default_options = {
        "boost/*:shared": True,
        "boost/*:without_cobalt_io_ssl": True,
        "gtest/*:shared": True,
        "boost/*:without_log": False,
        "boost/*:without_atomic": False,
        "boost/*:without_chrono": False,
        "boost/*:without_container": False,
        "boost/*:without_context": False,
        "boost/*:without_contract": True,
        "boost/*:without_coroutine": True,
        "boost/*:without_date_time": False,
        "boost/*:without_exception": False,
        "boost/*:without_fiber": True,
        "boost/*:without_filesystem": False,
        "boost/*:without_graph": True,
        "boost/*:without_graph_parallel": True,
        "boost/*:without_iostreams": True,
        "boost/*:without_json": True,
        "boost/*:without_locale": True,
        "boost/*:without_math": True,
        "boost/*:without_mpi": True,
        "boost/*:without_nowide": True,
        "boost/*:without_process": True,
        "boost/*:without_program_options": True,
        "boost/*:without_python": True,
        "boost/*:without_random": True,
        "boost/*:without_regex": False,
        "boost/*:without_serialization": True,
        "boost/*:without_stacktrace": True,
        "boost/*:without_system": True,
        "boost/*:without_test": True,
        "boost/*:without_thread": False,
        "boost/*:without_timer": True,
        "boost/*:without_type_erasure": True,
        "boost/*:without_url": True,
        "boost/*:without_wave": True,
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
