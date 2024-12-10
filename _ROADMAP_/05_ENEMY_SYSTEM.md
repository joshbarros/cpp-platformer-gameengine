# Phase 5: Enemy System - Detailed Design Document

## Overview
A comprehensive enemy system with various enemy types, AI behaviors, and combat mechanics.

## 1. Base Enemy Components

### Enemy Component
```cpp
struct EnemyComponent : Component {
    // Basic properties
    int Health = 1;
    float DetectionRange = 8.0f;
    float AttackRange = 1.0f;
    float MovementSpeed = 2.0f;
    
    // Combat properties
    int Damage = 1;
    float AttackCooldown = 1.0f;
    float CurrentAttackCooldown = 0.0f;
    
    // State tracking
    bool IsAggro = false;
    bool IsAttacking = false;
    Entity Target;
    
    virtual void OnHit(int damage);
    virtual void OnDeath();
};
```

### Enemy Types
```cpp
// Walker Enemy
struct WalkerEnemy : EnemyComponent {
    bool ShouldTurnAtEdge = true;
    float EdgeCheckDistance = 1.0f;
    bool MovingRight = true;
    
    void OnHit(int damage) override;
    void OnDeath() override;
};

// Jumper Enemy
struct JumperEnemy : EnemyComponent {
    float JumpForce = 15.0f;
    float JumpCooldown = 3.0f;
    float CurrentJumpCooldown = 0.0f;
    float JumpPredictionTime = 0.5f;
    
    void PredictJumpTarget(const Entity& target);
    void PerformJump();
};

// Flyer Enemy
struct FlyerEnemy : EnemyComponent {
    float HoverHeight = 4.0f;
    float WaveAmplitude = 2.0f;
    float WaveFrequency = 0.5f;
    float DiveSpeed = 8.0f;
    bool IsDiving = false;
    
    void UpdateFlight(float deltaTime);
    void InitiateDive();
};
```

## 2. Advanced Enemy Types

### Tank Enemy
```cpp
struct TankEnemy : EnemyComponent {
    // Shield mechanics
    bool ShieldActive = true;
    float ShieldRechargeTime = 3.0f;
    float CurrentShieldTime = 0.0f;
    float ShieldAngle = 180.0f;
    
    // Projectile properties
    struct Projectile {
        Vector2D Position;
        Vector2D Velocity;
        float Lifetime;
        int Damage;
    };
    
    std::vector<Projectile> ActiveProjectiles;
    float ProjectileCooldown = 4.0f;
    float CurrentProjectileCooldown = 0.0f;
    
    void FireProjectile();
    void UpdateProjectiles(float deltaTime);
    bool IsShieldFacingTarget(const Vector2D& targetPos);
};
```

### Spawner Enemy
```cpp
struct SpawnerEnemy : EnemyComponent {
    struct SpawnData {
        std::string EnemyType;
        float Weight;
    };
    
    std::vector<SpawnData> SpawnTypes;
    std::vector<Entity> ActiveSpawns;
    int MaxSpawns = 3;
    float SpawnCooldown = 5.0f;
    float CurrentSpawnCooldown = 0.0f;
    float ActivationRange = 12.0f;
    
    void SpawnEnemy();
    void UpdateSpawns();
    std::string SelectEnemyType();
};
```

### Mimic Enemy
```cpp
struct MimicEnemy : EnemyComponent {
    float DisguiseRange = 2.0f;
    float ChaseSpeed = 5.0f;
    float ChaseDuration = 3.0f;
    float CurrentChaseTime = 0.0f;
    bool IsDisguised = true;
    
    void UpdateDisguise(const Vector2D& playerPos);
    void StartChase();
    void UpdateChase(float deltaTime);
};
```

## 3. AI State Machine

### Enemy States
```cpp
namespace EnemyStates {
    class IdleState : public State {
        void Enter() override;
        void Update(float deltaTime) override;
        void Exit() override;
    };
    
    class PatrolState : public State {
        Vector2D m_PatrolStart;
        Vector2D m_PatrolEnd;
        bool m_MovingToEnd;
        
        void Enter() override;
        void Update(float deltaTime) override;
        void Exit() override;
    };
    
    class ChaseState : public State {
        void Enter() override;
        void Update(float deltaTime) override;
        void Exit() override;
    };
    
    class AttackState : public State {
        void Enter() override;
        void Update(float deltaTime) override;
        void Exit() override;
    };
    
    class RetreatState : public State {
        void Enter() override;
        void Update(float deltaTime) override;
        void Exit() override;
    };
}
```

## 4. Pathfinding System

### Path Finding
```cpp
class PathFinder {
private:
    struct Node {
        Vector2D Position;
        float G, H;
        Node* Parent;
    };
    
    std::vector<Vector2D> m_Waypoints;
    
public:
    std::vector<Vector2D> FindPath(
        const Vector2D& start,
        const Vector2D& end,
        const std::vector<CollisionShape*>& obstacles
    );
    
    void UpdatePath(
        const Vector2D& currentPos,
        const Vector2D& targetPos
    );
    
    Vector2D GetNextWaypoint() const;
    bool HasReachedWaypoint(const Vector2D& position) const;
};
```

### Movement System
```cpp
class EnemyMovementSystem : public System {
private:
    void UpdateWalker(Entity entity, float deltaTime);
    void UpdateJumper(Entity entity, float deltaTime);
    void UpdateFlyer(Entity entity, float deltaTime);
    void UpdateTank(Entity entity, float deltaTime);
    
public:
    void Update(Scene* scene, float deltaTime) override;
};
```

## 5. Combat System

### Attack Patterns
```cpp
class AttackPattern {
public:
    struct AttackData {
        int Damage;
        float Range;
        float Duration;
        float Cooldown;
    };
    
    virtual void Execute(Entity attacker, Entity target) = 0;
    virtual bool CanExecute() const = 0;
    virtual void Update(float deltaTime) = 0;
};

class MeleeAttack : public AttackPattern {
    void Execute(Entity attacker, Entity target) override;
};

class RangedAttack : public AttackPattern {
    void Execute(Entity attacker, Entity target) override;
};

class AreaAttack : public AttackPattern {
    void Execute(Entity attacker, Entity target) override;
};
```

## Implementation Timeline

### Week 1
- Day 1-2: Base enemy components
- Day 3-4: Basic enemy types (Walker, Jumper, Flyer)
- Day 5-7: Enemy state machine

### Week 2
- Day 1-2: Advanced enemy types (Tank, Spawner)
- Day 3-4: Pathfinding system
- Day 5-7: Combat system

### Week 3
- Day 1-2: AI behavior implementation
- Day 3-4: Enemy spawning and management
- Day 5-7: Testing and balancing

## Performance Targets

### AI Performance
- Path calculation < 1ms
- State updates < 0.1ms per enemy
- Maximum 50 active enemies
- Memory usage < 1KB per enemy

### Combat System
- Attack registration < 0.1ms
- Projectile updates < 0.5ms
- Collision checks < 1ms
- Memory usage < 100KB for combat system

## Testing Strategy

### Unit Tests
- Enemy state transitions
- Pathfinding accuracy
- Combat calculations
- Spawn system functionality
- AI decision making

### Integration Tests
- Enemy-player interaction
- Multi-enemy scenarios
- Combat system stress test
- Pathfinding edge cases
- State machine transitions

### Balance Testing
- Enemy difficulty curves
- Spawn rates and patterns
- Combat fairness
- Power scaling
- Resource management

## Debug Features

### Enemy Inspector
```cpp
class EnemyDebugger {
public:
    void ShowEnemyStats(Entity enemy);
    void ShowPathfinding(Entity enemy);
    void ShowAttackPatterns(Entity enemy);
    void ShowStateInfo(Entity enemy);
    void ShowSpawnInfo(Entity spawner);
};
```

### Visualization
- Enemy detection ranges
- Attack ranges
- Pathfinding waypoints
- State machine status
- Combat hitboxes

## Enemy Data Format (JSON)
```json
{
    "enemies": {
        "walker": {
            "health": 1,
            "speed": 2.0,
            "detection_range": 8.0,
            "attack_range": 1.0,
            "damage": 1,
            "turn_at_edge": true
        },
        "jumper": {
            "health": 1,
            "jump_force": 15.0,
            "jump_cooldown": 3.0,
            "prediction_time": 0.5
        },
        "flyer": {
            "health": 2,
            "hover_height": 4.0,
            "wave_amplitude": 2.0,
            "wave_frequency": 0.5,
            "dive_speed": 8.0
        }
    }
}
```
