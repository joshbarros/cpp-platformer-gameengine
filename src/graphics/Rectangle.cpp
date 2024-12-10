#include "graphics/Rectangle.hpp"
#include <glad/glad.h>
#include <array>

Rectangle::Rectangle(float x, float y, float width, float height, const glm::vec4& color)
    : m_X(x)
    , m_Y(y)
    , m_Width(width)
    , m_Height(height)
    , m_Color(color)
    , m_VAO(0)
    , m_VBO(0)
    , m_Initialized(false) {
    InitializeGL();
}

void Rectangle::InitializeGL() {
    if (m_Initialized) return;

    // Create and bind VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Define vertices (x, y)
    std::array<float, 12> vertices = {
        0.0f, 0.0f,      // Bottom left
        m_Width, 0.0f,   // Bottom right
        m_Width, m_Height,// Top right
        0.0f, 0.0f,      // Bottom left
        m_Width, m_Height,// Top right
        0.0f, m_Height   // Top left
    };

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_Initialized = true;
}

void Rectangle::Draw() const {
    // Use a basic shader program (you'll need to create this)
    // For now, we'll assume a basic shader that takes position and color uniforms
    
    // Set color uniform
    // shader.setVec4("color", m_Color);
    
    // Set model matrix uniform for position
    // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_X, m_Y, 0.0f));
    // shader.setMat4("model", model);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Rectangle::SetPosition(float x, float y) {
    m_X = x;
    m_Y = y;
}

void Rectangle::Move(float dx, float dy) {
    m_X += dx;
    m_Y += dy;
}

void Rectangle::SetColor(const glm::vec4& color) {
    m_Color = color;
}
