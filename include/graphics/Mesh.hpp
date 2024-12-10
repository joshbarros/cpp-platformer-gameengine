#pragma once
#include "Vertex.hpp"
#include "Shader.hpp"
#include <vector>
#include <memory>

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Draw(const Shader& shader) const;
    
    // Getters
    bool IsValid() const { return m_VAO != 0; }
    size_t GetVertexCount() const { return m_Vertices.size(); }
    size_t GetIndexCount() const { return m_Indices.size(); }
    
    // Prevent copying
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    
    // Allow moving
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

private:
    void SetupMesh();
    void CleanupMesh();

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    unsigned int m_VAO{0}, m_VBO{0}, m_EBO{0};
};
