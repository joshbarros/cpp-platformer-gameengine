cmake_minimum_required(VERSION 3.15)

# Set CMake policies
cmake_policy(SET CMP0135 NEW)

# Set language before project declaration
enable_language(C CXX)

project(PlatformerEngine VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Enable testing
enable_testing()

# Include FetchContent for downloading dependencies
include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# On macOS, use the system OpenAL framework
if(APPLE)
    find_library(OPENAL_LIBRARY OpenAL REQUIRED)
else()
    find_package(OpenAL REQUIRED)
endif()

# Add GLAD
add_library(glad STATIC
    external/glad/src/glad.c
)
target_include_directories(glad PUBLIC external/glad/include)

# Add stb_image include directory
include_directories(${CMAKE_SOURCE_DIR}/external/stb)

# Source files
set(SOURCES
    src/main.cpp
    src/core/Engine.cpp
    src/core/Window.cpp
    src/core/Input.cpp
    src/core/Timer.cpp
    src/core/Logger.cpp
    src/core/ResourceManager.cpp
    src/graphics/Mesh.cpp
    src/graphics/Texture.cpp
    src/graphics/Shader.cpp
    src/graphics/Renderer.cpp
    src/graphics/stb_image_impl.cpp
    src/utils/Debug.cpp
)

# Header files
set(HEADERS
    include/core/Engine.hpp
    include/core/Window.hpp
    include/core/Input.hpp
    include/core/Timer.hpp
    include/core/Logger.hpp
    include/core/Resource.hpp
    include/core/ResourceManager.hpp
    include/graphics/Mesh.hpp
    include/graphics/Texture.hpp
    include/graphics/Shader.hpp
    include/graphics/Renderer.hpp
    include/graphics/Vertex.hpp
    include/utils/Debug.hpp
)

# Create library target for the engine
add_library(${PROJECT_NAME}Lib STATIC ${SOURCES} ${HEADERS})

# Set include directories for the library
target_include_directories(${PROJECT_NAME}Lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/stb
        ${CMAKE_SOURCE_DIR}/external/glad/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries to the engine library
target_link_libraries(${PROJECT_NAME}Lib
    PUBLIC
        OpenGL::GL
        glfw
        glm::glm
        glad
        ${OPENAL_LIBRARY}
)

# Create executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the executable with our engine library
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}Lib
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Test files
set(TEST_SOURCES
    tests/core/ResourceManagerTests.cpp
    tests/graphics/VertexTests.cpp
    tests/graphics/MeshTests.cpp
    tests/graphics/TextureTests.cpp
    tests/graphics/RendererTests.cpp
)

# Create test executable
add_executable(${PROJECT_NAME}Tests ${TEST_SOURCES})

# Link the test executable with our engine library
target_link_libraries(${PROJECT_NAME}Tests
    PRIVATE
        ${PROJECT_NAME}Lib
        gtest_main
)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION bin/assets
)
