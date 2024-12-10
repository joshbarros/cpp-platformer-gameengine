#include "core/Window.hpp"
#include "core/Logger.hpp"

Window::Window()
    : m_Window(nullptr) {
}

Window::~Window() {
    Shutdown();
}

bool Window::Init(const Properties& props) {
    m_Properties = props;
    
    // Initialize GLFW
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }
    
    // Set error callback
    glfwSetErrorCallback(ErrorCallback);
    
    // Set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // Create window
    m_Window = glfwCreateWindow(
        m_Properties.Width,
        m_Properties.Height,
        m_Properties.Title.c_str(),
        m_Properties.Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr
    );
    
    if (!m_Window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    
    // Make OpenGL context current
    glfwMakeContextCurrent(m_Window);
    
    // Set VSync
    glfwSwapInterval(m_Properties.VSync ? 1 : 0);
    
    LOG_INFO("Window created successfully: {}x{}", m_Properties.Width, m_Properties.Height);
    return true;
}

void Window::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}

void Window::Update() {
    glfwPollEvents();
}

void Window::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

void Window::SetResizeCallback(GLFWwindowsizefun callback) {
    glfwSetWindowSizeCallback(m_Window, callback);
}

void Window::SetKeyCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(m_Window, callback);
}

void Window::SetMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(m_Window, callback);
}

void Window::SetCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(m_Window, callback);
}

void Window::SetScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(m_Window, callback);
}

void Window::ErrorCallback(int error, const char* description) {
    LOG_ERROR("GLFW Error ({}): {}", error, description);
}
