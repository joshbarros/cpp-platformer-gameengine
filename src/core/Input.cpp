#include "core/Input.hpp"
#include "core/Window.hpp"
#include "core/Logger.hpp"
#include "utils/Debug.hpp"

Input* Input::s_Instance = nullptr;

Input::Input(Window* window)
    : m_Window(window)
    , m_MouseX(0.0)
    , m_MouseY(0.0)
    , m_LastMouseX(0.0)
    , m_LastMouseY(0.0)
    , m_MouseScrollDelta(0.0)
    , m_FirstMouse(true) {
    
    ASSERT(window != nullptr, "Window cannot be null");
    ASSERT(s_Instance == nullptr, "Input system already exists");
    s_Instance = this;
    
    // Initialize key states
    std::fill(m_KeyStates.begin(), m_KeyStates.end(), KeyState::Released);
    std::fill(m_MouseButtonStates.begin(), m_MouseButtonStates.end(), KeyState::Released);
    
    // Set callbacks
    GLFWwindow* glfwWindow = window->GetNativeWindow();
    glfwSetKeyCallback(glfwWindow, KeyCallback);
    glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
    glfwSetCursorPosCallback(glfwWindow, CursorPosCallback);
    glfwSetScrollCallback(glfwWindow, ScrollCallback);
    
    LOG_INFO("Input system initialized");
}

void Input::Update() {
    UpdateKeyStates();
    UpdateMouseButtonStates();
    UpdateActionStates();
    
    // Reset scroll delta
    m_MouseScrollDelta = 0.0;
}

void Input::UpdateKeyStates() {
    for (size_t i = 0; i < m_KeyStates.size(); ++i) {
        if (m_KeyStates[i] == KeyState::JustReleased) {
            m_KeyStates[i] = KeyState::Released;
        }
        else if (m_KeyStates[i] == KeyState::Pressed) {
            m_KeyStates[i] = KeyState::Held;
        }
    }
}

void Input::UpdateMouseButtonStates() {
    for (size_t i = 0; i < m_MouseButtonStates.size(); ++i) {
        if (m_MouseButtonStates[i] == KeyState::JustReleased) {
            m_MouseButtonStates[i] = KeyState::Released;
        }
        else if (m_MouseButtonStates[i] == KeyState::Pressed) {
            m_MouseButtonStates[i] = KeyState::Held;
        }
    }
}

void Input::UpdateActionStates() {
    for (auto& [name, action] : m_Actions) {
        action.isActive = false;
        
        // Check keys
        for (int key : action.keys) {
            if (IsKeyPressed(key) || IsKeyHeld(key)) {
                action.isActive = true;
                if (action.callback) {
                    action.callback();
                }
                break;
            }
        }
        
        // Check mouse buttons
        if (!action.isActive) {
            for (MouseButton button : action.mouseButtons) {
                if (IsMouseButtonPressed(button) || IsMouseButtonHeld(button)) {
                    action.isActive = true;
                    if (action.callback) {
                        action.callback();
                    }
                    break;
                }
            }
        }
    }
}

bool Input::IsKeyPressed(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return false;
    return m_KeyStates[key] == KeyState::Pressed;
}

bool Input::IsKeyHeld(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return false;
    return m_KeyStates[key] == KeyState::Held;
}

bool Input::IsKeyReleased(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return false;
    return m_KeyStates[key] == KeyState::Released;
}

bool Input::IsKeyJustReleased(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return false;
    return m_KeyStates[key] == KeyState::JustReleased;
}

KeyState Input::GetKeyState(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return KeyState::Released;
    return m_KeyStates[key];
}

bool Input::IsMouseButtonPressed(MouseButton button) const {
    size_t index = static_cast<size_t>(button);
    if (index >= static_cast<size_t>(MouseButton::Count)) return false;
    return m_MouseButtonStates[index] == KeyState::Pressed;
}

bool Input::IsMouseButtonHeld(MouseButton button) const {
    size_t index = static_cast<size_t>(button);
    if (index >= static_cast<size_t>(MouseButton::Count)) return false;
    return m_MouseButtonStates[index] == KeyState::Held;
}

bool Input::IsMouseButtonReleased(MouseButton button) const {
    size_t index = static_cast<size_t>(button);
    if (index >= static_cast<size_t>(MouseButton::Count)) return false;
    return m_MouseButtonStates[index] == KeyState::Released;
}

bool Input::IsMouseButtonJustReleased(MouseButton button) const {
    size_t index = static_cast<size_t>(button);
    if (index >= static_cast<size_t>(MouseButton::Count)) return false;
    return m_MouseButtonStates[index] == KeyState::JustReleased;
}

KeyState Input::GetMouseButtonState(MouseButton button) const {
    size_t index = static_cast<size_t>(button);
    if (index >= static_cast<size_t>(MouseButton::Count)) return KeyState::Released;
    return m_MouseButtonStates[index];
}

void Input::GetMousePosition(double& x, double& y) const {
    x = m_MouseX;
    y = m_MouseY;
}

void Input::GetMouseDelta(double& dx, double& dy) {
    dx = m_MouseX - m_LastMouseX;
    dy = m_LastMouseY - m_MouseY; // Inverted Y for intuitive camera control
    
    m_LastMouseX = m_MouseX;
    m_LastMouseY = m_MouseY;
}

double Input::GetMouseScrollDelta() const {
    return m_MouseScrollDelta;
}

void Input::MapAction(const std::string& name, const std::vector<int>& keys,
                     const std::vector<MouseButton>& mouseButtons) {
    InputAction action;
    action.name = name;
    action.keys = keys;
    action.mouseButtons = mouseButtons;
    action.isActive = false;
    
    m_Actions[name] = action;
    LOG_INFO("Mapped action: {}", name);
}

void Input::UnmapAction(const std::string& name) {
    auto it = m_Actions.find(name);
    if (it != m_Actions.end()) {
        m_Actions.erase(it);
        LOG_INFO("Unmapped action: {}", name);
    }
}

void Input::BindActionCallback(const std::string& name, std::function<void()> callback) {
    auto it = m_Actions.find(name);
    if (it != m_Actions.end()) {
        it->second.callback = callback;
    }
}

bool Input::IsActionActive(const std::string& name) const {
    auto it = m_Actions.find(name);
    return it != m_Actions.end() && it->second.isActive;
}

void Input::SetCursorMode(int mode) {
    glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, mode);
}

void Input::ShowCursor() {
    SetCursorMode(GLFW_CURSOR_NORMAL);
}

void Input::HideCursor() {
    SetCursorMode(GLFW_CURSOR_HIDDEN);
}

void Input::LockCursor() {
    SetCursorMode(GLFW_CURSOR_DISABLED);
}

// Static callback functions
void Input::KeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (s_Instance && key >= 0 && key < GLFW_KEY_LAST) {
        switch (action) {
            case GLFW_PRESS:
                s_Instance->m_KeyStates[key] = KeyState::Pressed;
                break;
            case GLFW_RELEASE:
                s_Instance->m_KeyStates[key] = KeyState::JustReleased;
                break;
            case GLFW_REPEAT:
                s_Instance->m_KeyStates[key] = KeyState::Held;
                break;
        }
    }
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/) {
    if (s_Instance && button >= 0 && button < static_cast<int>(MouseButton::Count)) {
        switch (action) {
            case GLFW_PRESS:
                s_Instance->m_MouseButtonStates[button] = KeyState::Pressed;
                break;
            case GLFW_RELEASE:
                s_Instance->m_MouseButtonStates[button] = KeyState::JustReleased;
                break;
        }
    }
}

void Input::CursorPosCallback(GLFWwindow* window, double x, double y) {
    if (s_Instance) {
        if (s_Instance->m_FirstMouse) {
            s_Instance->m_LastMouseX = x;
            s_Instance->m_LastMouseY = y;
            s_Instance->m_FirstMouse = false;
        }
        
        s_Instance->m_MouseX = x;
        s_Instance->m_MouseY = y;
    }
}

void Input::ScrollCallback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    if (s_Instance) {
        s_Instance->m_MouseScrollDelta = yoffset;
    }
}
