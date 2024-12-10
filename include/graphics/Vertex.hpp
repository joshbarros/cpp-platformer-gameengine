#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position;  // Position in 3D space
    glm::vec2 TexCoords; // Texture coordinates
    glm::vec4 Color;     // Vertex color
    
    Vertex() = default;
    
    Vertex(const glm::vec3& position, const glm::vec2& texCoords = glm::vec2(0.0f), 
           const glm::vec4& color = glm::vec4(1.0f))
        : Position(position), TexCoords(texCoords), Color(color) {}
};
