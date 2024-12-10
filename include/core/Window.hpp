#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    struct Properties {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool VSync;
        bool Fullscreen;
    };
    
    Window();
    ~Window();
    
    // Delete copy constructor and assignment operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    
    bool Init(const Properties& props);
    void Shutdown();
    void Update();
    
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    void SwapBuffers();
    bool ShouldClose() const;
    
    // Getters
    uint32_t GetWidth() const { return m_Properties.Width; }
    uint32_t GetHeight() const { return m_Properties.Height; }
    float GetAspectRatio() const { return static_cast<float>(m_Properties.Width) / static_cast<float>(m_Properties.Height); }
    GLFWwindow* GetNativeWindow() const { return m_Window; }
    
    // Event callbacks
    void SetResizeCallback(GLFWwindowsizefun callback);
    void SetKeyCallback(GLFWkeyfun callback);
    void SetMouseButtonCallback(GLFWmousebuttonfun callback);
    void SetCursorPosCallback(GLFWcursorposfun callback);
    void SetScrollCallback(GLFWscrollfun callback);
    
private:
    static void ErrorCallback(int error, const char* description);
    
    GLFWwindow* m_Window;
    Properties m_Properties;
};
