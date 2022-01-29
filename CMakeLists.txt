cmake_minimum_required(VERSION 3.21)
project(Grafika)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES src/shadersSources/firstSquare/squareFrag.shader
        src/shadersSources/firstSquare/squareVertex.shader src/main.cpp src/glad.c
        headers/Constants.h src/Vector2i.cpp headers/Vector2i.h src/Colors.cpp
        headers/Colors.h src/Constants.cpp src/Shader.cpp headers/Shader.h src/Texture.cpp
        headers/Texture.h src/stb_image.cpp headers/stb_image.h
        src/Engine3D.cpp src/Vector2.cpp src/Sphere.cpp)

include_directories(${PROJECT_SOURCE_DIR}/GLFW/include ${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/GLFW/lib-mingw-w64)

add_executable(Grafika ${SOURCE_FILES})

target_link_libraries(Grafika glfw3 gdi32 opengl32)
