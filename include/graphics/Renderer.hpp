#pragma once
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

class Renderer {
public:
    static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }

    void Init();
    void Shutdown();

    // Basic rendering functions
    void Clear(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    void DrawMesh(const Mesh& mesh, const Shader& shader);
    void DrawRectangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    void DrawTexturedRectangle(const glm::vec2& position, const glm::vec2& size, 
                             const Texture& texture, const glm::vec4& tint = glm::vec4(1.0f));

    // Camera and transformation
    void SetProjectionMatrix(const glm::mat4& projection);
    void SetViewMatrix(const glm::mat4& view);
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

private:
    Renderer();
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void CreateDefaultShaders();
    void CreateDefaultMeshes();

    // Matrices
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

    // Default resources
    std::shared_ptr<Shader> m_ColorShader;
    std::shared_ptr<Shader> m_TextureShader;
    std::unique_ptr<Mesh> m_QuadMesh;

    bool m_Initialized;
};
