cmake_minimum_required(VERSION 3.12)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_path(PORTABLE_FILE_DIALOGS_INCLUDE_DIRS "portable-file-dialogs.h")

file(GLOB_RECURSE SOURCE_LIST "src/*.cpp" "src/*.hpp" "src/*.h")

add_library(Simple-Engine STATIC ${SOURCE_LIST})

target_include_directories(Simple-Engine
    PUBLIC
        src
        ${PORTABLE_FILE_DIALOGS_INCLUDE_DIRS}
)

target_link_libraries(Simple-Engine
    PUBLIC
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        GLEW::GLEW
        glm::glm
        imgui::imgui
)