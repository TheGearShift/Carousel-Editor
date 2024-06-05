cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

# Set the output directories for executable, library, and archive files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Statically link MSVC Runtime Library
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# If vcpkg present as submodule, bring in the toolchain
if(EXISTS ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
    message(STATUS "Found vcpkg.cmake, using it!")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")
endif()

# Enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Project)

add_subdirectory(Simple-Engine)
add_subdirectory(Model-Editor)