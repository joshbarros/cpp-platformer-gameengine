#pragma once
#include <glm/glm.hpp>

class Rectangle {
public:
    Rectangle(float x, float y, float width, float height, const glm::vec4& color);
    ~Rectangle() = default;

    void Draw() const;
    void SetPosition(float x, float y);
    void Move(float dx, float dy);
    void SetColor(const glm::vec4& color);
    
    // Getters
    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }
    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }
    const glm::vec4& GetColor() const { return m_Color; }

private:
    void InitializeGL();

    float m_X, m_Y;
    float m_Width, m_Height;
    glm::vec4 m_Color;
    unsigned int m_VAO, m_VBO;
    bool m_Initialized;
};
