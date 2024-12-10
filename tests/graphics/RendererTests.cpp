#include <gtest/gtest.h>
#include "graphics/Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>

class RendererTests : public ::testing::Test {
protected:
    void SetUp() override {
        renderer = &Renderer::getInstance();
        renderer->Init();
    }

    void TearDown() override {
        renderer->Shutdown();
    }

    Renderer* renderer;
};

TEST_F(RendererTests, Initialization) {
    // Since we've already initialized in SetUp, we can test if it's working
    EXPECT_TRUE(renderer != nullptr);
}

TEST_F(RendererTests, ClearScreen) {
    // Test clearing the screen (this mainly tests if the function doesn't crash)
    renderer->Clear();
    // If we got here without crashing, the test passes
}

TEST_F(RendererTests, ProjectionMatrix) {
    // Create and set a projection matrix
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    renderer->SetProjectionMatrix(projection);
    
    // Verify that the matrix was set correctly
    EXPECT_EQ(renderer->GetProjectionMatrix(), projection);
}
