# 2D Platformer - Game Engine

A modern C++ game engine implementation of a Super Mario World-style platformer, featuring advanced physics, enemy AI, and power-up systems.

## Build Requirements

- CMake 3.15 or higher
- C++17 compatible compiler
- GLFW3
- OpenGL
- OpenAL
- GLM

### macOS Setup
```bash
# Install dependencies using Homebrew
brew install cmake
brew install glfw
brew install glm
brew install openal-soft
```

### Linux Setup
```bash
# Ubuntu/Debian
sudo apt-get install cmake
sudo apt-get install libglfw3-dev
sudo apt-get install libglm-dev
sudo apt-get install libopenal-dev

# Fedora
sudo dnf install cmake
sudo dnf install glfw-devel
sudo dnf install glm-devel
sudo dnf install openal-soft-devel
```

## Building the Project

```bash
# Clone the repository
git clone https://github.com/yourusername/cpp-platformer-gameengine.git
cd cpp-platformer-gameengine

# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```

## Project Status

### Phase 1: Core Engine Setup (Completed)
- [x] Project Setup
  - [x] Initialize CMake project structure
  - [x] Configure build system
  - [x] Add external dependencies (GLFW, OpenGL, OpenAL, GLM)
  - [x] Create basic directory structure
  - [x] Set up unit testing (Google Test)
- [x] Core Systems
  - [x] Logger implementation
  - [x] Debug/Assert system
  - [x] Time management system
- [x] Window Management
  - [x] GLFW window creation
  - [x] OpenGL context setup
  - [x] Basic event handling
  - [x] Window configuration options
- [x] Input System
  - [x] Keyboard input handling
  - [x] Mouse input handling
  - [x] Input mapping system
  - [ ] Gamepad support (Coming soon)
- [x] Resource Management
  - [x] Resource loading system
  - [x] Resource caching
  - [x] Memory management
  - [x] Error handling

### Phase 2: Rendering System (Next Up)
- [ ] Basic Rendering
  - [ ] Shader system
  - [ ] Texture loading
  - [ ] Sprite rendering
  - [ ] Batch rendering
- [ ] Advanced Rendering
  - [ ] Particle system
  - [ ] Lighting system
  - [ ] Post-processing effects

## Controls

The current implementation supports the following controls:
- **A/LEFT Arrow**: Move left
- **D/RIGHT Arrow**: Move right
- **SPACE**: Jump
- **S/DOWN Arrow**: Crouch
- **LEFT SHIFT**: Run
- **ESC**: Exit game

## Project Structure

```
cpp-platformer-gameengine/
├── assets/            # Game assets (textures, audio, shaders)
├── include/           # Header files
│   ├── core/         # Core engine systems
│   └── utils/        # Utility classes and functions
├── src/              # Source files
│   ├── core/         # Core engine implementations
│   └── utils/        # Utility implementations
├── tests/            # Unit tests
├── CMakeLists.txt    # Main CMake configuration
└── README.md         # This file
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.
