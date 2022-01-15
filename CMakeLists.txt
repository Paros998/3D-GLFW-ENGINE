cmake_minimum_required(VERSION 3.21)
project(Grafika)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES src/main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/GLFW/include)
link_directories(${PROJECT_SOURCE_DIR}/GLFW/lib-mingw-w64)

add_executable(Grafika ${SOURCE_FILES})

target_link_libraries(Grafika glfw3 gdi32 opengl32)
