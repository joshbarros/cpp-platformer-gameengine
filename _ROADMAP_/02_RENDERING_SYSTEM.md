# Phase 2: Rendering System - Detailed Design Document

## Overview
The rendering system handles all graphics-related tasks, from basic sprite rendering to advanced visual effects.

## 1. Shader System

### Shader Class Design
```cpp
class Shader {
private:
    uint32_t m_ProgramID;
    std::unordered_map<std::string, int> m_UniformLocations;
    
public:
    void Load(const std::string& vertexPath, const std::string& fragmentPath);
    void Bind() const;
    void Unbind() const;
    
    // Uniform setters
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVector2f(const std::string& name, const glm::vec2& value);
    void SetVector3f(const std::string& name, const glm::vec3& value);
    void SetVector4f(const std::string& name, const glm::vec4& value);
    void SetMatrix4(const std::string& name, const glm::mat4& value);
};
```

### Default Shaders
```glsl
// Sprite Vertex Shader
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec2 TexCoord;
out vec4 Color;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Color = aColor;
}

// Sprite Fragment Shader
#version 450 core
in vec2 TexCoord;
in vec4 Color;

uniform sampler2D uTexture;
uniform float uAlpha;

out vec4 FragColor;

void main() {
    FragColor = texture(uTexture, TexCoord) * Color;
    FragColor.a *= uAlpha;
}
```

## 2. Texture System

### Texture Class
```cpp
class Texture {
private:
    uint32_t m_TextureID;
    int m_Width, m_Height;
    int m_Channels;
    
public:
    void Load(const std::string& path);
    void LoadFromMemory(const void* data, int width, int height, int channels);
    void Bind(uint32_t slot = 0) const;
    void Unbind() const;
    
    // Texture parameters
    void SetFilterMode(GLenum min, GLenum mag);
    void SetWrapMode(GLenum s, GLenum t);
    void GenerateMipmaps();
};
```

### Texture Atlas
```cpp
class TextureAtlas {
private:
    std::unique_ptr<Texture> m_Texture;
    std::unordered_map<std::string, Rectangle> m_Regions;
    
public:
    void Load(const std::string& texturePath, const std::string& atlasData);
    const Rectangle& GetRegion(const std::string& name) const;
    void BindRegion(const std::string& name);
};
```

## 3. Sprite Batch Renderer

### Vertex Structure
```cpp
struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec4 Color;
    float TexIndex;
};

class SpriteBatch {
private:
    static const uint32_t MaxSprites = 10000;
    static const uint32_t MaxVertices = MaxSprites * 4;
    static const uint32_t MaxIndices = MaxSprites * 6;
    
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::vector<Texture*> m_TextureSlots;
    
public:
    void Begin(const Camera& camera);
    void Draw(const Sprite& sprite);
    void End();
    void Flush();
};
```

## 4. Camera System

### Camera Class
```cpp
class Camera {
private:
    glm::vec2 m_Position;
    float m_Rotation;
    float m_Zoom;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    
public:
    void SetPosition(const glm::vec2& position);
    void SetRotation(float rotation);
    void SetZoom(float zoom);
    void SetViewport(int width, int height);
    
    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
};
```

## 5. Animation System

### Animation Class
```cpp
class Animation {
public:
    struct Frame {
        Rectangle Region;
        float Duration;
    };
    
private:
    std::vector<Frame> m_Frames;
    float m_CurrentTime;
    int m_CurrentFrame;
    bool m_Loop;
    
public:
    void Update(float deltaTime);
    const Frame& GetCurrentFrame() const;
    void Reset();
    void SetLoop(bool loop);
};
```

### Animator Component
```cpp
class Animator {
private:
    std::unordered_map<std::string, Animation> m_Animations;
    Animation* m_CurrentAnimation;
    
public:
    void AddAnimation(const std::string& name, const Animation& animation);
    void Play(const std::string& name);
    void Update(float deltaTime);
    void Stop();
};
```

## 6. Particle System

### Particle Structure
```cpp
struct Particle {
    glm::vec2 Position;
    glm::vec2 Velocity;
    glm::vec4 Color;
    float Size;
    float Life;
    float MaxLife;
};

class ParticleSystem {
private:
    std::vector<Particle> m_Particles;
    Texture* m_Texture;
    
public:
    void Emit(const ParticleProps& props);
    void Update(float deltaTime);
    void Render();
};
```

## 7. Layer System

### Layer Management
```cpp
class Layer {
public:
    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnRender() = 0;
};

class LayerStack {
private:
    std::vector<Layer*> m_Layers;
    
public:
    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);
    void Update(float deltaTime);
    void Render();
};
```

## Implementation Timeline

### Week 1
- Day 1-2: Shader system implementation
- Day 3-4: Texture loading and management
- Day 5-7: Basic sprite rendering

### Week 2
- Day 1-2: Sprite batch renderer
- Day 3-4: Camera system
- Day 5-7: Animation system

### Week 3
- Day 1-2: Particle system
- Day 3-4: Layer system
- Day 5-7: Visual effects and optimization

## Performance Targets

### Rendering Performance
- 60 FPS with 10,000 sprites
- Batch size: 1000 sprites per draw call
- Texture atlas support up to 4096x4096
- Maximum 32 texture slots
- Particle system: 10,000 particles at 60 FPS

### Memory Usage
- Vertex buffer: 1MB
- Index buffer: 256KB
- Texture memory: 64MB max
- Shader compilation < 100ms

## Testing Strategy

### Unit Tests
- Shader compilation and uniform setting
- Texture loading and binding
- Camera transformations
- Animation frame calculations
- Particle system updates

### Visual Tests
- Sprite rendering accuracy
- Animation smoothness
- Particle effects
- Layer ordering
- Camera movement and zoom

### Performance Tests
- Batch renderer efficiency
- Texture atlas performance
- Particle system stress test
- Memory usage monitoring
- FPS consistency test
