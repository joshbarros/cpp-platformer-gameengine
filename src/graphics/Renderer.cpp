#include "graphics/Renderer.hpp"
#include "core/Logger.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {
    const char* colorVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 2) in vec4 aColor;
        
        uniform mat4 projection;
        uniform mat4 view;
        uniform mat4 model;
        
        out vec4 vertexColor;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

    const char* colorFragmentShader = R"(
        #version 330 core
        in vec4 vertexColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vertexColor;
        }
    )";

    const char* textureVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;
        
        uniform mat4 projection;
        uniform mat4 view;
        uniform mat4 model;
        
        out vec2 TexCoord;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* textureFragmentShader = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        
        uniform sampler2D texture1;
        uniform vec4 tintColor;
        
        void main() {
            FragColor = texture(texture1, TexCoord) * tintColor;
        }
    )";
}

Renderer::Renderer()
    : m_ProjectionMatrix(1.0f), m_ViewMatrix(1.0f), m_Initialized(false) {
}

Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Init() {
    if (m_Initialized) {
        Logger::Warn("Renderer already initialized");
        return;
    }

    CreateDefaultShaders();
    CreateDefaultMeshes();

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Initialized = true;
    Logger::Info("Renderer initialized successfully");
}

void Renderer::Shutdown() {
    if (!m_Initialized) return;

    m_ColorShader.reset();
    m_TextureShader.reset();
    m_QuadMesh.reset();

    m_Initialized = false;
    Logger::Info("Renderer shut down");
}

void Renderer::CreateDefaultShaders() {
    // Create color shader
    m_ColorShader = std::make_shared<Shader>();
    if (!m_ColorShader->Init(colorVertexShader, colorFragmentShader)) {
        Logger::Error("Failed to create color shader");
        return;
    }

    // Create texture shader
    m_TextureShader = std::make_shared<Shader>();
    if (!m_TextureShader->Init(textureVertexShader, textureFragmentShader)) {
        Logger::Error("Failed to create texture shader");
        return;
    }
}

void Renderer::CreateDefaultMeshes() {
    // Create quad mesh
    std::vector<Vertex> quadVertices = {
        Vertex({-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}),
        Vertex({-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f})
    };

    std::vector<unsigned int> quadIndices = {
        0, 1, 2,
        2, 3, 0
    };

    m_QuadMesh = std::make_unique<Mesh>(quadVertices, quadIndices);
}

void Renderer::Clear(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawMesh(const Mesh& mesh, const Shader& shader) {
    shader.Use();
    shader.SetMat4("projection", m_ProjectionMatrix);
    shader.SetMat4("view", m_ViewMatrix);
    shader.SetMat4("model", glm::mat4(1.0f));
    mesh.Draw(shader);
}

void Renderer::DrawRectangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    m_ColorShader->Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    m_ColorShader->SetMat4("projection", m_ProjectionMatrix);
    m_ColorShader->SetMat4("view", m_ViewMatrix);
    m_ColorShader->SetMat4("model", model);
    
    // Update quad vertices color
    std::vector<Vertex> coloredQuad = {
        Vertex({-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, color),
        Vertex({ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, color),
        Vertex({ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}, color),
        Vertex({-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}, color)
    };
    
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
    Mesh coloredQuadMesh(coloredQuad, indices);
    coloredQuadMesh.Draw(*m_ColorShader);
}

void Renderer::DrawTexturedRectangle(const glm::vec2& position, const glm::vec2& size, 
                                   const Texture& texture, const glm::vec4& tint) {
    m_TextureShader->Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    m_TextureShader->SetMat4("projection", m_ProjectionMatrix);
    m_TextureShader->SetMat4("view", m_ViewMatrix);
    m_TextureShader->SetMat4("model", model);
    m_TextureShader->SetVec4("tintColor", tint);
    
    texture.Bind();
    m_QuadMesh->Draw(*m_TextureShader);
    texture.Unbind();
}

void Renderer::SetProjectionMatrix(const glm::mat4& projection) {
    m_ProjectionMatrix = projection;
}

void Renderer::SetViewMatrix(const glm::mat4& view) {
    m_ViewMatrix = view;
}
