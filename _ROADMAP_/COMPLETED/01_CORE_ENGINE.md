# Phase 1: Core Engine Setup - Detailed Design Document

## Overview
This phase establishes the foundational architecture of the game engine, setting up essential systems and project structure.

## 1. Project Setup

### CMake Configuration
```cmake
# Minimum required version
cmake_minimum_required(VERSION 3.15)

# Required dependencies
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenAL REQUIRED)
```

### Directory Structure
```
src/
├── core/
│   ├── Engine.hpp/cpp       # Main engine class
│   ├── Window.hpp/cpp       # GLFW window wrapper
│   ├── Logger.hpp/cpp       # Logging system
│   ├── Timer.hpp/cpp        # Time management
│   └── Memory.hpp/cpp       # Memory management
├── utils/
│   ├── Debug.hpp/cpp        # Debug utilities
│   ├── Math.hpp/cpp         # Math utilities
│   └── Config.hpp/cpp       # Configuration system
└── main.cpp                 # Entry point
```

### External Dependencies
- GLFW 3.3+
- OpenGL 4.5
- GLM 0.9.9+
- OpenAL 1.1
- stb_image
- spdlog (for logging)
- json (for configuration)

## 2. Window Management

### Window Class Design
```cpp
class Window {
private:
    GLFWwindow* m_Window;
    WindowProps m_Properties;
    
public:
    struct WindowProps {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool VSync;
        bool Fullscreen;
    };
    
    // Core functionality
    bool Init(const WindowProps& props);
    void Shutdown();
    void Update();
    void SwapBuffers();
    
    // Event callbacks
    void SetResizeCallback(GLFWwindowsizefun callback);
    void SetKeyCallback(GLFWkeyfun callback);
    void SetMouseCallback(GLFWcursorenterfun callback);
};
```

### OpenGL Context
- Version: 4.5 Core Profile
- Debug context in development builds
- Vsync control
- Multi-sampling support
- Hardware info querying

## 3. Core Systems

### Game Loop
```cpp
class Engine {
private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Timer> m_Timer;
    bool m_Running;
    
    // Fixed timestep variables
    const double m_FixedTimeStep = 1.0 / 60.0;
    double m_Accumulator = 0.0;
    
public:
    void Run() {
        while (m_Running) {
            m_Timer->Update();
            double deltaTime = m_Timer->GetDeltaTime();
            m_Accumulator += deltaTime;
            
            // Fixed update for physics
            while (m_Accumulator >= m_FixedTimeStep) {
                FixedUpdate(m_FixedTimeStep);
                m_Accumulator -= m_FixedTimeStep;
            }
            
            // Variable update for rendering
            Update(deltaTime);
            Render();
        }
    }
};
```

### Time Management
```cpp
class Timer {
private:
    double m_LastTime;
    double m_DeltaTime;
    double m_TimeScale;
    
public:
    void Update();
    double GetDeltaTime() const;
    double GetTimeScale() const;
    void SetTimeScale(double scale);
    double GetTotalTime() const;
};
```

### Logger System
```cpp
class Logger {
public:
    enum class Level {
        Trace,
        Info,
        Warning,
        Error,
        Critical
    };
    
    static void Init();
    static void Shutdown();
    
    template<typename... Args>
    static void Log(Level level, const char* fmt, Args&&... args);
    
    // Convenience methods
    template<typename... Args>
    static void Info(const char* fmt, Args&&... args);
    
    template<typename... Args>
    static void Error(const char* fmt, Args&&... args);
};
```

### Memory Management
```cpp
class MemoryManager {
private:
    struct MemoryStats {
        size_t TotalAllocated;
        size_t TotalFreed;
        size_t CurrentUsage;
        size_t PeakUsage;
    };
    
    MemoryStats m_Stats;
    
public:
    void* Allocate(size_t size);
    void Free(void* ptr);
    const MemoryStats& GetStats() const;
    
    // Memory pools for frequently allocated objects
    template<typename T>
    class Pool {
        // Implementation for object pooling
    };
};
```

## 4. Error Handling

### Error System
```cpp
class Error {
public:
    enum class Code {
        None,
        WindowCreationFailed,
        OpenGLContextFailed,
        ShaderCompilationFailed,
        TextureLoadFailed,
        AudioDeviceFailed
    };
    
    static void SetCallback(const std::function<void(Code, const char*)>& callback);
    static void Report(Code code, const char* message);
};
```

### Debug Features
- Debug build configuration
- Assert macros
- Performance counters
- Memory tracking
- OpenGL debug callback

## 5. Configuration System

### Config File Structure (JSON)
```json
{
    "window": {
        "title": "Platform Game",
        "width": 1280,
        "height": 720,
        "vsync": true,
        "fullscreen": false
    },
    "graphics": {
        "msaa": 4,
        "maxFPS": 60,
        "shadows": true,
        "particleCount": 1000
    },
    "audio": {
        "masterVolume": 1.0,
        "musicVolume": 0.8,
        "sfxVolume": 1.0
    },
    "debug": {
        "showFPS": true,
        "showColliders": false,
        "logLevel": "info"
    }
}
```

## Implementation Timeline

### Week 1
- Day 1-2: Project setup and CMake configuration
- Day 3-4: Window management and OpenGL context
- Day 5-7: Core systems (Timer, Logger)

### Week 2
- Day 1-2: Memory management system
- Day 3-4: Error handling and debug features
- Day 5-7: Configuration system and testing

## Testing Strategy

### Unit Tests
- Window creation/destruction
- Timer accuracy
- Memory leaks
- Configuration loading/saving
- Logger functionality

### Integration Tests
- Window events with engine loop
- Memory management under load
- Error handling scenarios
- Configuration changes at runtime

## Performance Targets
- Engine initialization < 2 seconds
- Memory footprint < 50MB at startup
- Zero memory leaks
- Stable 60 FPS with vsync
- Logger overhead < 1% CPU time
