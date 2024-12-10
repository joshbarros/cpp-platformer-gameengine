#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    ~Shader();

    bool Init(const std::string& vertexSource, const std::string& fragmentSource);
    void Use() const;
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
    bool CompileShader(unsigned int& shader, const std::string& source, unsigned int type);
    unsigned int m_Program;
};
