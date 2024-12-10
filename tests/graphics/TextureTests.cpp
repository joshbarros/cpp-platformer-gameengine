#include <gtest/gtest.h>
#include "graphics/Texture.hpp"

class TextureTests : public ::testing::Test {
protected:
    void SetUp() override {
        testTexturePath = "tests/assets/test_texture.png";
    }

    std::string testTexturePath;
};

TEST_F(TextureTests, ValidTextureLoading) {
    Texture texture;
    bool loaded = texture.loadFromFile(testTexturePath);
    EXPECT_TRUE(loaded);
    EXPECT_NE(texture.GetID(), 0);
    EXPECT_GT(texture.GetWidth(), 0);
    EXPECT_GT(texture.GetHeight(), 0);
}

TEST_F(TextureTests, InvalidTextureLoading) {
    Texture texture;
    bool loaded = texture.loadFromFile("nonexistent.png");
    EXPECT_FALSE(loaded);
    EXPECT_EQ(texture.GetID(), 0);
}
