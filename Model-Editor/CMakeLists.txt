cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE SOURCE_LIST "src/*.cpp" "src/*.hpp" "src/*.h")

add_executable(Model-Editor ${SOURCE_LIST})

target_include_directories(Model-Editor
    PRIVATE
        src
)

target_link_libraries(Model-Editor
    PRIVATE
        Simple-Engine
)