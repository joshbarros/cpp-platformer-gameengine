#pragma once
#include "core/Resource.hpp"
#include <string>

class Texture : public Resource {
public:
    Texture();
    ~Texture();

    // Implement Resource interface
    bool loadFromFile(const std::string& path) override;

    // Texture-specific functionality
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    // Getters
    unsigned int GetID() const { return m_TextureID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetChannels() const { return m_Channels; }

private:
    void Cleanup();

    unsigned int m_TextureID;
    int m_Width;
    int m_Height;
    int m_Channels;
};
