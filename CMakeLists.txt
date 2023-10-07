cmake_minimum_required(VERSION 3.25)

project(
    "KIT_Modbus"
    VERSION 1.0.0
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory(shared)
add_subdirectory(server)
add_subdirectory(client)
