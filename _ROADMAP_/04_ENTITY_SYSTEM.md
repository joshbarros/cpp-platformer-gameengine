# Phase 4: Entity System - Detailed Design Document

## Overview
A component-based entity system that provides flexibility and modularity for game objects.

## 1. Core ECS Framework

### Entity Class
```cpp
class Entity {
private:
    Scene* m_Scene;
    entt::entity m_Handle;
    
public:
    template<typename T, typename... Args>
    T& AddComponent(Args&&... args);
    
    template<typename T>
    T& GetComponent();
    
    template<typename T>
    bool HasComponent() const;
    
    template<typename T>
    void RemoveComponent();
    
    uint32_t GetID() const;
    bool IsValid() const;
};
```

### Component Base
```cpp
struct Component {
    Entity Owner;
    bool Active = true;
    
    virtual ~Component() = default;
    virtual void OnStart() {}
    virtual void OnDestroy() {}
};
```

### Scene Management
```cpp
class Scene {
private:
    entt::registry m_Registry;
    std::vector<System*> m_Systems;
    
public:
    Entity CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(Entity entity);
    
    void Update(float deltaTime);
    void FixedUpdate(float fixedDeltaTime);
    void Render();
    
    template<typename T>
    void RegisterSystem();
};
```

## 2. Basic Components

### Transform Component
```cpp
struct TransformComponent : Component {
    Vector2D Position = Vector2D::Zero;
    float Rotation = 0.0f;
    Vector2D Scale = Vector2D::One;
    
    Matrix4x4 GetTransformMatrix() const;
    void SetParent(Entity parent);
    std::vector<Entity> GetChildren() const;
};
```

### Sprite Component
```cpp
struct SpriteComponent : Component {
    Texture* Texture = nullptr;
    Rectangle SourceRect;
    Vector2D Offset = Vector2D::Zero;
    Vector4D Color = Vector4D(1.0f);
    int LayerOrder = 0;
    bool FlipX = false;
    bool FlipY = false;
};
```

### Physics Component
```cpp
struct PhysicsComponent : Component {
    RigidBody* Body = nullptr;
    CollisionShape* Shape = nullptr;
    
    bool IsTrigger = false;
    bool FixedRotation = true;
    float Mass = 1.0f;
    float Friction = 0.2f;
    float Restitution = 0.0f;
    
    void OnCollisionEnter(Entity other);
    void OnCollisionExit(Entity other);
    void OnTriggerEnter(Entity other);
    void OnTriggerExit(Entity other);
};
```

### Animation Component
```cpp
struct AnimationComponent : Component {
    struct AnimationClip {
        std::string Name;
        std::vector<Rectangle> Frames;
        float FrameTime;
        bool Loop;
    };
    
    std::unordered_map<std::string, AnimationClip> Animations;
    std::string CurrentAnimation;
    float CurrentTime = 0.0f;
    int CurrentFrame = 0;
    bool Playing = false;
    
    void Play(const std::string& name);
    void Stop();
    void Update(float deltaTime);
};
```

## 3. Player Implementation

### Player Component
```cpp
struct PlayerComponent : Component {
    // Movement properties
    float MoveSpeed = 6.0f;
    float JumpForce = 18.0f;
    float MaxJumpTime = 0.35f;
    float CoyoteTime = 0.15f;
    float JumpBufferTime = 0.1f;
    
    // State tracking
    bool IsGrounded = false;
    bool IsJumping = false;
    float CurrentJumpTime = 0.0f;
    float LastGroundedTime = 0.0f;
    float LastJumpPressedTime = 0.0f;
    
    // Power-up state
    bool HasSpeedPowerup = false;
    bool HasJumpPowerup = false;
    int PowerupTimeRemaining = 0;
};
```

### Player Controller System
```cpp
class PlayerControllerSystem : public System {
private:
    void HandleInput(Entity player, float deltaTime);
    void UpdateMovement(Entity player, float deltaTime);
    void UpdateJump(Entity player, float deltaTime);
    void UpdateAnimation(Entity player);
    void CheckGrounded(Entity player);
    
public:
    void Update(Scene* scene, float deltaTime) override;
};
```

## 4. Collision Component

### Collision Handler
```cpp
class CollisionHandler {
public:
    struct CollisionData {
        Entity EntityA;
        Entity EntityB;
        Vector2D Normal;
        float Penetration;
    };
    
    void OnCollisionEnter(const CollisionData& data);
    void OnCollisionStay(const CollisionData& data);
    void OnCollisionExit(const CollisionData& data);
    
    void OnTriggerEnter(Entity trigger, Entity other);
    void OnTriggerExit(Entity trigger, Entity other);
};
```

## 5. State Machine Component

### State Machine
```cpp
class StateMachine : public Component {
public:
    class State {
    public:
        virtual void Enter() = 0;
        virtual void Exit() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void FixedUpdate(float fixedDeltaTime) = 0;
    };
    
private:
    std::unordered_map<std::string, std::unique_ptr<State>> m_States;
    State* m_CurrentState;
    
public:
    void AddState(const std::string& name, std::unique_ptr<State> state);
    void ChangeState(const std::string& name);
    void Update(float deltaTime);
    void FixedUpdate(float fixedDeltaTime);
};
```

## Implementation Timeline

### Week 1
- Day 1-2: Core ECS framework
- Day 3-4: Basic components implementation
- Day 5-7: Component communication system

### Week 2
- Day 1-2: Player component and controller
- Day 3-4: Collision handling system
- Day 5-7: State machine implementation

### Week 3
- Day 1-2: Animation system integration
- Day 3-4: Physics integration
- Day 5-7: Testing and optimization

## Performance Targets

### Entity Management
- Support for 1000+ entities
- Component access < 100ns
- Entity creation < 1ms
- Memory overhead < 64 bytes per entity

### Systems Performance
- Update cycle < 1ms for 1000 entities
- Collision detection < 2ms per frame
- Memory usage < 100MB for full scene

## Testing Strategy

### Unit Tests
- Entity creation/destruction
- Component addition/removal
- System registration
- State machine transitions
- Collision detection

### Integration Tests
- Player movement
- Physics interaction
- Animation transitions
- Component communication
- Scene management

### Performance Tests
- Entity stress test
- Component access speed
- Memory usage monitoring
- Update cycle timing
- Scene loading time

## Debug Features

### Entity Inspector
```cpp
class EntityInspector {
public:
    void ShowEntity(Entity entity);
    void ShowComponents(Entity entity);
    void EditComponent(Component* component);
    void ShowSystemStatus();
    void ShowSceneHierarchy();
};
```

### Debug Visualization
- Entity boundaries
- Collision shapes
- Component relationships
- State machine status
- System dependencies

## Scene Serialization

### Scene Format (JSON)
```json
{
    "entities": [
        {
            "id": 1,
            "name": "Player",
            "components": {
                "Transform": {
                    "position": {"x": 0, "y": 0},
                    "rotation": 0,
                    "scale": {"x": 1, "y": 1}
                },
                "Sprite": {
                    "texture": "player.png",
                    "layer": 1
                },
                "Physics": {
                    "mass": 1.0,
                    "fixedRotation": true
                },
                "Player": {
                    "moveSpeed": 6.0,
                    "jumpForce": 18.0
                }
            }
        }
    ]
}
```

### Prefab System
```cpp
class Prefab {
private:
    std::string m_Name;
    json m_Data;
    
public:
    Entity Instantiate(Scene* scene);
    void SaveToFile(const std::string& path);
    static Prefab LoadFromFile(const std::string& path);
};
```
