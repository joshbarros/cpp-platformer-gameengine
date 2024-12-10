#include "graphics/Shader.hpp"
#include "core/Logger.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : m_Program(0) {}

Shader::~Shader() {
    if (m_Program != 0) {
        glDeleteProgram(m_Program);
    }
}

bool Shader::Init(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int vertexShader = 0, fragmentShader = 0;
    
    // Compile vertex shader
    if (!CompileShader(vertexShader, vertexSource, GL_VERTEX_SHADER)) {
        return false;
    }
    
    // Compile fragment shader
    if (!CompileShader(fragmentShader, fragmentSource, GL_FRAGMENT_SHADER)) {
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Create shader program
    m_Program = glCreateProgram();
    glAttachShader(m_Program, vertexShader);
    glAttachShader(m_Program, fragmentShader);
    glLinkProgram(m_Program);
    
    // Check linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
        LOG_ERROR("Shader program linking failed: {}", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    // Delete shaders as they're linked into our program and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

bool Shader::CompileShader(unsigned int& shader, const std::string& source, unsigned int type) {
    shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("Shader compilation failed: {}", infoLog);
        return false;
    }
    return true;
}

void Shader::Use() const {
    glUseProgram(m_Program);
}

void Shader::SetBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
    glUniform4fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(value));
}
