cmake_minimum_required(VERSION 3.10)

project(OBJViewer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directories for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Use vcpkg to find packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED CONFIG)
find_package(glfw3 REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
find_package(stb REQUIRED CONFIG)
find_package(FreeGLUT REQUIRED CONFIG)

# Source files
set(SOURCES
    Main.cpp
    InputManager.cpp
    Shader.cpp
    Texture.cpp
    # Add other source files as needed
)

# Add executable
add_executable(OBJViewer ${SOURCES})

# Link libraries
target_link_libraries(OBJViewer PRIVATE OpenGL::GL GLEW::GLEW glfw glm::glm stb::stb freeglut::freeglut)
