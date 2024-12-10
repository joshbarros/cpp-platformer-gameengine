# Phase 3: Physics System - Detailed Design Document

## Overview
A custom 2D physics engine designed specifically for platformer mechanics, focusing on precise collision detection and response.

## 1. Core Math Components

### Vector2D Class
```cpp
class Vector2D {
private:
    float x, y;
    
public:
    // Basic operations
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator/(float scalar) const;
    
    // Utility functions
    float Length() const;
    float LengthSquared() const;
    Vector2D Normalize() const;
    float Dot(const Vector2D& other) const;
    float Cross(const Vector2D& other) const;
    
    // Common vectors
    static const Vector2D Zero;
    static const Vector2D One;
    static const Vector2D Up;
    static const Vector2D Right;
};
```

### Transform Class
```cpp
class Transform {
private:
    Vector2D m_Position;
    float m_Rotation;
    Vector2D m_Scale;
    
public:
    void SetPosition(const Vector2D& position);
    void SetRotation(float rotation);
    void SetScale(const Vector2D& scale);
    
    Matrix3x3 GetTransformMatrix() const;
};
```

## 2. Collision Detection

### AABB Structure
```cpp
struct AABB {
    Vector2D Min;
    Vector2D Max;
    
    bool Intersects(const AABB& other) const;
    bool Contains(const Vector2D& point) const;
    AABB GetExpanded(float amount) const;
};
```

### Collision Shapes
```cpp
class CollisionShape {
public:
    enum class Type {
        Box,
        Circle,
        Polygon
    };
    
    virtual Type GetType() const = 0;
    virtual AABB GetBoundingBox() const = 0;
    virtual bool TestPoint(const Vector2D& point) const = 0;
};

class BoxShape : public CollisionShape {
private:
    Vector2D m_HalfExtents;
    
public:
    Type GetType() const override { return Type::Box; }
    AABB GetBoundingBox() const override;
    bool TestPoint(const Vector2D& point) const override;
};
```

### Collision Detection System
```cpp
struct CollisionManifold {
    bool HasCollision;
    Vector2D Normal;
    float Penetration;
    std::vector<Vector2D> ContactPoints;
};

class CollisionDetector {
public:
    static CollisionManifold TestCollision(
        const CollisionShape& shapeA,
        const Transform& transformA,
        const CollisionShape& shapeB,
        const Transform& transformB
    );
    
private:
    static CollisionManifold BoxToBox(
        const BoxShape& boxA,
        const Transform& transformA,
        const BoxShape& boxB,
        const Transform& transformB
    );
};
```

## 3. Spatial Partitioning

### QuadTree Implementation
```cpp
class QuadTree {
private:
    struct Node {
        AABB Bounds;
        std::vector<PhysicsBody*> Bodies;
        std::array<std::unique_ptr<Node>, 4> Children;
        bool IsLeaf;
    };
    
    std::unique_ptr<Node> m_Root;
    int m_MaxDepth;
    int m_MaxBodiesPerNode;
    
public:
    void Insert(PhysicsBody* body);
    void Remove(PhysicsBody* body);
    std::vector<PhysicsBody*> Query(const AABB& area);
    void Update();
};
```

## 4. Physics Body

### RigidBody Class
```cpp
class RigidBody {
private:
    Vector2D m_Position;
    Vector2D m_Velocity;
    Vector2D m_Force;
    float m_Mass;
    float m_Restitution;
    float m_Friction;
    
    // Platform-specific properties
    bool m_IsGrounded;
    bool m_IsOnSlope;
    float m_SlopeAngle;
    
public:
    void ApplyForce(const Vector2D& force);
    void ApplyImpulse(const Vector2D& impulse);
    void Update(float deltaTime);
    
    // Platformer-specific methods
    void UpdateGroundState(const CollisionManifold& manifold);
    bool CanJump() const;
};
```

## 5. Physics World

### PhysicsWorld Class
```cpp
class PhysicsWorld {
private:
    std::vector<RigidBody*> m_Bodies;
    QuadTree m_BroadPhase;
    float m_Gravity;
    int m_VelocityIterations;
    int m_PositionIterations;
    
public:
    void Step(float deltaTime);
    void AddBody(RigidBody* body);
    void RemoveBody(RigidBody* body);
    
private:
    void ResolveCollisions();
    void IntegrateVelocities(float deltaTime);
    void IntegratePositions(float deltaTime);
};
```

## 6. Platform-Specific Features

### Platform Types
```cpp
class Platform {
public:
    enum class Type {
        Static,
        OneWay,
        Moving,
        Falling,
        Ice,
        Conveyor
    };
    
    virtual Type GetType() const = 0;
    virtual void Update(float deltaTime) = 0;
};

class MovingPlatform : public Platform {
private:
    Vector2D m_StartPosition;
    Vector2D m_EndPosition;
    float m_Speed;
    float m_Progress;
    
public:
    Type GetType() const override { return Type::Moving; }
    void Update(float deltaTime) override;
};
```

### Ray Casting
```cpp
struct RaycastHit {
    bool Hit;
    Vector2D Point;
    Vector2D Normal;
    float Distance;
    Platform* HitPlatform;
};

class RayCaster {
public:
    static RaycastHit Cast(
        const Vector2D& origin,
        const Vector2D& direction,
        float maxDistance,
        const PhysicsWorld& world
    );
};
```

## Implementation Timeline

### Week 1
- Day 1-2: Math components and utilities
- Day 3-4: Basic collision shapes and detection
- Day 5-7: AABB implementation and testing

### Week 2
- Day 1-2: QuadTree spatial partitioning
- Day 3-4: RigidBody implementation
- Day 5-7: Physics world and integration

### Week 3
- Day 1-2: Platform-specific features
- Day 3-4: Ray casting and ground detection
- Day 5-7: Testing and optimization

## Performance Targets

### Physics Simulation
- 60 Hz fixed timestep
- Support for 1000+ static colliders
- Up to 100 dynamic bodies
- Collision detection < 1ms per frame
- Memory usage < 50MB

### Platform-Specific
- Precise ground detection
- Smooth slope handling
- Responsive jump mechanics
- Accurate moving platform behavior
- Efficient one-way platform detection

## Testing Strategy

### Unit Tests
- Vector and math operations
- AABB intersection tests
- Collision detection accuracy
- Ray casting precision
- QuadTree operations

### Integration Tests
- Physics body movement
- Collision response
- Platform interactions
- Gravity and forces
- Memory management

### Platformer-Specific Tests
- Jump mechanics
- Slope handling
- Moving platform attachment
- One-way platform behavior
- Ice physics
- Conveyor belt effects

## Debug Visualization

### Debug Renderer
```cpp
class PhysicsDebugRenderer {
public:
    void DrawCollider(const CollisionShape& shape, const Transform& transform);
    void DrawAABB(const AABB& aabb);
    void DrawQuadTree(const QuadTree& quadTree);
    void DrawRaycast(const Vector2D& start, const Vector2D& end, bool hit);
    void DrawContactPoint(const Vector2D& point, const Vector2D& normal);
};
```

### Debug Options
- Collision shapes
- Velocity vectors
- Contact points and normals
- QuadTree boundaries
- Ray casts
- Ground detection rays
- Platform types
