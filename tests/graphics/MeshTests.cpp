#include <gtest/gtest.h>
#include "graphics/Mesh.hpp"
#include <vector>

class MeshTests : public ::testing::Test {
protected:
    void SetUp() override {
        vertices = {
            Vertex(glm::vec3(-0.5f, -0.5f, 0.0f)),
            Vertex(glm::vec3(0.5f, -0.5f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.5f, 0.0f))
        };
        indices = {0, 1, 2};
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

TEST_F(MeshTests, Construction) {
    Mesh mesh(vertices, indices);
    EXPECT_TRUE(mesh.IsValid());
    EXPECT_EQ(mesh.GetVertexCount(), 3);
    EXPECT_EQ(mesh.GetIndexCount(), 3);
}

TEST_F(MeshTests, InvalidConstruction) {
    std::vector<Vertex> emptyVertices;
    std::vector<unsigned int> emptyIndices;
    Mesh mesh(emptyVertices, emptyIndices);
    EXPECT_FALSE(mesh.IsValid());
}
