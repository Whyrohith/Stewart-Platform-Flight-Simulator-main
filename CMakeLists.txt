cmake_minimum_required(VERSION 3.22)
project(spf_simulator64)

set(CMAKE_CXX_STANDARD 11)
if (WIN32)
    set(EXECUTABLE_OUTPUT_PATH ../build/)
else()
    set(EXECUTABLE_OUTPUT_PATH ../build-linux/)
endif (WIN32)

add_compile_options(-O3)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
include_directories(spf_simulator64 "src" "external/include")
add_executable(spf_simulator64 ${SRC_FILES})

if (WIN32)
    target_link_libraries(spf_simulator64 "${CMAKE_CURRENT_SOURCE_DIR}/external/lib/libamogpu.a" mingw32 SDL2main SDL2 opengl32 glew32 freetype)
else()
    target_link_libraries(spf_simulator64 "${CMAKE_CURRENT_SOURCE_DIR}/external/lib/linux/libamogpu.a" SDL2main SDL2 GL GLEW freetype)
endif (WIN32)