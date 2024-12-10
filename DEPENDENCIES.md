# Development Environment Setup

This document explains how to set up your development environment for building the PlatformerEngine project on Linux and macOS.

## Required Dependencies

- CMake (3.15 or higher)
- C++17 compatible compiler (GCC, Clang)
- GLFW3
- OpenGL
- OpenAL
- GLM (OpenGL Mathematics)

## macOS Setup

1. Install Homebrew (if not already installed):
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. Install the required dependencies:
```bash
# Install CMake
brew install cmake

# Install GLFW
brew install glfw

# Install GLM
brew install glm

# Install OpenAL-Soft (OpenAL implementation)
brew install openal-soft
```

3. Set up environment variables (add to ~/.zshrc or ~/.bash_profile):
```bash
# OpenAL environment variables
export LDFLAGS="-L/opt/homebrew/opt/openal-soft/lib"
export CPPFLAGS="-I/opt/homebrew/opt/openal-soft/include"
```

## Linux Setup (Ubuntu/Debian)

1. Update package list:
```bash
sudo apt update
```

2. Install build tools:
```bash
sudo apt install build-essential cmake
```

3. Install dependencies:
```bash
# Install OpenGL dependencies
sudo apt install libgl1-mesa-dev

# Install GLFW
sudo apt install libglfw3-dev

# Install GLM
sudo apt install libglm-dev

# Install OpenAL
sudo apt install libopenal-dev
```

## Linux Setup (Fedora)

1. Install build tools:
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake
```

2. Install dependencies:
```bash
# Install OpenGL dependencies
sudo dnf install mesa-libGL-devel

# Install GLFW
sudo dnf install glfw-devel

# Install GLM
sudo dnf install glm-devel

# Install OpenAL
sudo dnf install openal-soft-devel
```

## Linux Setup (Arch Linux)

1. Install build tools:
```bash
sudo pacman -S base-devel cmake
```

2. Install dependencies:
```bash
sudo pacman -S mesa
sudo pacman -S glfw-x11
sudo pacman -S glm
sudo pacman -S openal
```

## Verifying Installation

To verify that all dependencies are properly installed, you can run:

```bash
# Check CMake version
cmake --version

# Check OpenGL (on Linux)
glxinfo | grep "OpenGL version"

# Check GLFW
pkg-config --modversion glfw3

# Check OpenAL
# On Linux:
pkg-config --modversion openal
# On macOS:
brew list openal-soft --version
```

## Building the Project

After installing all dependencies:

```bash
# Create build directory
mkdir build && cd build

# Configure project
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build project
cmake --build .

# Run tests
ctest --output-on-failure
```

## Troubleshooting

### Common Issues on macOS

1. OpenGL deprecation warnings:
   - These warnings are expected on macOS 10.14 and later
   - Add `-DGL_SILENCE_DEPRECATION` to silence them

2. OpenAL framework issues:
   - Make sure environment variables are properly set
   - Check if OpenAL-Soft is properly linked

### Common Issues on Linux

1. GLFW not found:
   - Ensure pkg-config is installed
   - Check if GLFW development files are installed

2. OpenGL headers not found:
   - Install mesa development packages
   - Check if GL/gl.h exists in system include paths

3. Linking errors:
   - Make sure all development (-dev/-devel) packages are installed
   - Check if library paths are correctly set

## Additional Notes

- The project uses C++17 features, so make sure your compiler supports this standard
- For development, it's recommended to also install:
  - Git for version control
  - A C++ IDE or code editor (VS Code, CLion, etc.)
  - GDB or LLDB for debugging
