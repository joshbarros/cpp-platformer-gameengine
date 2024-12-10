cmake_minimum_required(VERSION 3.15)
project(PlatformerEngine VERSION 1.0.0 LANGUAGES CXX)

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

# Source files
set(SOURCES
    src/core/Engine.cpp
    src/core/Window.cpp
    src/core/Logger.cpp
    src/core/Timer.cpp
    src/core/Input.cpp
    src/core/ResourceManager.cpp
    src/utils/Debug.cpp
)

# Header files
set(HEADERS
    include/core/Engine.hpp
    include/core/Window.hpp
    include/core/Logger.hpp
    include/core/Timer.hpp
    include/core/Input.hpp
    include/core/Resource.hpp
    include/core/ResourceManager.hpp
    include/utils/Debug.hpp
)

# Create library target for the engine
add_library(${PROJECT_NAME}Lib STATIC ${SOURCES} ${HEADERS})

# Set include directories for the library
target_include_directories(${PROJECT_NAME}Lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries to the engine library
target_link_libraries(${PROJECT_NAME}Lib
    PUBLIC
        OpenGL::GL
        glfw
        glm::glm
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

# Enable testing
enable_testing()
add_subdirectory(tests)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION bin/assets
)
