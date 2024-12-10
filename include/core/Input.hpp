#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <array>
#include <string>
#include <functional>

// Forward declarations
class Window;

enum class KeyState {
    Released,
    Pressed,
    Held,
    JustReleased
};

enum class MouseButton {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    Button4 = GLFW_MOUSE_BUTTON_4,
    Button5 = GLFW_MOUSE_BUTTON_5,
    Count
};

struct InputAction {
    std::string name;
    std::vector<int> keys;
    std::vector<MouseButton> mouseButtons;
    std::function<void()> callback;
    bool isActive;
};

class Input {
public:
    Input(Window* window);
    ~Input() = default;
    
    // Delete copy constructor and assignment operator
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    
    void Update();
    
    // Keyboard input
    bool IsKeyPressed(int key) const;
    bool IsKeyHeld(int key) const;
    bool IsKeyReleased(int key) const;
    bool IsKeyJustReleased(int key) const;
    KeyState GetKeyState(int key) const;
    
    // Mouse input
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsMouseButtonHeld(MouseButton button) const;
    bool IsMouseButtonReleased(MouseButton button) const;
    bool IsMouseButtonJustReleased(MouseButton button) const;
    KeyState GetMouseButtonState(MouseButton button) const;
    
    void GetMousePosition(double& x, double& y) const;
    void GetMouseDelta(double& dx, double& dy);
    double GetMouseScrollDelta() const;
    
    // Input mapping
    void MapAction(const std::string& name, const std::vector<int>& keys, 
                  const std::vector<MouseButton>& mouseButtons = {});
    void UnmapAction(const std::string& name);
    void BindActionCallback(const std::string& name, std::function<void()> callback);
    bool IsActionActive(const std::string& name) const;
    
    // Input mode
    void SetCursorMode(int mode);
    void ShowCursor();
    void HideCursor();
    void LockCursor();
    
private:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double x, double y);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    void UpdateKeyStates();
    void UpdateMouseButtonStates();
    void UpdateActionStates();
    
    Window* m_Window;
    static Input* s_Instance;
    
    std::array<KeyState, GLFW_KEY_LAST + 1> m_KeyStates;
    std::array<KeyState, static_cast<size_t>(MouseButton::Count)> m_MouseButtonStates;
    std::unordered_map<std::string, InputAction> m_Actions;
    
    double m_MouseX;
    double m_MouseY;
    double m_LastMouseX;
    double m_LastMouseY;
    double m_MouseScrollDelta;
    bool m_FirstMouse;
};
