#include <gtest/gtest.h>
#include "graphics/Vertex.hpp"
#include <glm/glm.hpp>

TEST(VertexTests, DefaultConstructor) {
    Vertex vertex;
    EXPECT_EQ(vertex.Position, glm::vec3(0.0f));
    EXPECT_EQ(vertex.TexCoords, glm::vec2(0.0f));
    EXPECT_EQ(vertex.Color, glm::vec4(1.0f));
}

TEST(VertexTests, ParameterizedConstructor) {
    glm::vec3 pos(1.0f, 2.0f, 3.0f);
    glm::vec2 tex(0.5f, 0.5f);
    glm::vec4 col(1.0f, 0.0f, 0.0f, 1.0f);
    
    Vertex vertex(pos, tex, col);
    EXPECT_EQ(vertex.Position, pos);
    EXPECT_EQ(vertex.TexCoords, tex);
    EXPECT_EQ(vertex.Color, col);
}
