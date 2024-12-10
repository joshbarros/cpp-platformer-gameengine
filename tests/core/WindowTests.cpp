#include <gtest/gtest.h>
#include "core/Window.hpp"

class WindowTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize GLFW for all tests
        ASSERT_TRUE(glfwInit());
    }
    
    void TearDown() override {
        // Terminate GLFW after all tests
        glfwTerminate();
    }
};

TEST_F(WindowTests, Creation) {
    Window window;
    Window::Properties props{
        .Title = "Test Window",
        .Width = 800,
        .Height = 600,
        .VSync = true,
        .Fullscreen = false
    };
    
    EXPECT_TRUE(window.Init(props));
    EXPECT_EQ(window.GetWidth(), 800);
    EXPECT_EQ(window.GetHeight(), 600);
    EXPECT_FLOAT_EQ(window.GetAspectRatio(), 800.0f / 600.0f);
    EXPECT_NE(window.GetNativeWindow(), nullptr);
    
    window.Shutdown();
}

TEST_F(WindowTests, ShouldClose) {
    Window window;
    Window::Properties props{
        .Title = "Test Window",
        .Width = 800,
        .Height = 600,
        .VSync = true,
        .Fullscreen = false
    };
    
    ASSERT_TRUE(window.Init(props));
    EXPECT_FALSE(window.ShouldClose());
    
    // Simulate window close
    glfwSetWindowShouldClose(window.GetNativeWindow(), GLFW_TRUE);
    EXPECT_TRUE(window.ShouldClose());
    
    window.Shutdown();
}
