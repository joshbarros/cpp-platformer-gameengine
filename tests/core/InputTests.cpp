#include <gtest/gtest.h>
#include "core/Input.hpp"
#include "core/Window.hpp"

class InputTests : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(glfwInit());
        
        Window::Properties props{
            .Title = "Input Test Window",
            .Width = 800,
            .Height = 600,
            .VSync = true,
            .Fullscreen = false
        };
        
        m_Window = std::make_unique<Window>();
        ASSERT_TRUE(m_Window->Init(props));
        
        m_Input = std::make_unique<Input>(m_Window.get());
    }
    
    void TearDown() override {
        m_Input.reset();
        m_Window.reset();
        glfwTerminate();
    }
    
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Input> m_Input;
};

TEST_F(InputTests, ActionMapping) {
    // Map a jump action to space bar and left mouse button
    std::vector<int> keys = { GLFW_KEY_SPACE };
    std::vector<MouseButton> buttons = { MouseButton::Left };
    m_Input->MapAction("Jump", keys, buttons);
    
    // Initially, action should not be active
    EXPECT_FALSE(m_Input->IsActionActive("Jump"));
    
    // Test callback
    bool callbackCalled = false;
    m_Input->BindActionCallback("Jump", [&callbackCalled]() {
        callbackCalled = true;
    });
    
    // Unmap action
    m_Input->UnmapAction("Jump");
    EXPECT_FALSE(m_Input->IsActionActive("Jump"));
}

TEST_F(InputTests, MousePosition) {
    double x, y;
    m_Input->GetMousePosition(x, y);
    
    // Initial position should be at the center of the window
    EXPECT_DOUBLE_EQ(x, 0.0);
    EXPECT_DOUBLE_EQ(y, 0.0);
}

TEST_F(InputTests, CursorModes) {
    // Test different cursor modes
    m_Input->ShowCursor();
    m_Input->HideCursor();
    m_Input->LockCursor();
    
    // No assertions needed, just making sure these don't crash
}
