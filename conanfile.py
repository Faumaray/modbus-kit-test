import os
from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import copy

class KIT_modbus(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    build_policy = "missing"

    def requirements(self):
        self.requires("libmodbus/3.1.8")
        self.requires("asio/1.28.1")
        self.requires("nlohmann_json/3.11.2")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = 1
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install(["out"])

    def layout(self):
        cmake_layout(self)
        self.cpp.package.prefix = f"{self.package_folder}/out"
