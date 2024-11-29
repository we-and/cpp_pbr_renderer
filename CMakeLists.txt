cmake_minimum_required(VERSION 3.10)
project(PBRRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/opt/homebrew/opt/glew/include)
include_directories(/opt/homebrew/opt/glfw/include)
include_directories(/opt/homebrew/Cellar/glm/1.0.1/include)
# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add glad source
add_library(glad STATIC     lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)

add_library(tinygltf STATIC     lib/tinygltf/tiny_gltf.cc)

# Main executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/PBRRenderer.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinygltf
    ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    tinygltf
    
    /opt/homebrew/opt/glfw/lib/libglfw.3.dylib
    /opt/homebrew/Cellar/glm/1.0.1/lib/libglm.dylib
    ${OPENGL_LIBRARIES})