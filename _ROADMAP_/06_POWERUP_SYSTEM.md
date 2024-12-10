# Phase 6: Power-Up System - Detailed Design Document

## Overview
A flexible power-up system that enhances player abilities and provides temporary or permanent status effects.

## 1. Base Power-Up System

### PowerUp Component
```cpp
class PowerUpComponent : public Component {
public:
    enum class Type {
        SpeedBell,
        BoomerangSuit,
        CloneCherry,
        FireFlower,
        PropellerBox
    };
    
    struct Properties {
        Type PowerUpType;
        bool IsPermanent;
        float Duration;
        float CollectionAnimationTime;
        float SpawnAnimationTime;
        bool CanStack;
        int MaxStacks;
    };
    
private:
    Properties m_Properties;
    bool m_IsActive;
    float m_RemainingTime;
    int m_CurrentStacks;
    
public:
    virtual void OnCollect(Entity collector);
    virtual void OnActivate();
    virtual void OnDeactivate();
    virtual void Update(float deltaTime);
    
    bool IsActive() const { return m_IsActive; }
    float GetRemainingTime() const { return m_RemainingTime; }
};
```

### Power-Up Factory
```cpp
class PowerUpFactory {
private:
    std::unordered_map<PowerUpComponent::Type, std::function<Entity(Scene*)>> m_Creators;
    
public:
    void RegisterPowerUp(
        PowerUpComponent::Type type,
        std::function<Entity(Scene*)> creator
    );
    
    Entity CreatePowerUp(
        Scene* scene,
        PowerUpComponent::Type type,
        const Vector2D& position
    );
};
```

## 2. Specific Power-Ups

### Speed Bell
```cpp
class SpeedBellPowerUp : public PowerUpComponent {
private:
    float m_SpeedMultiplier = 1.5f;
    float m_DashForce = 3.0f;
    float m_DashCooldown = 1.0f;
    float m_CurrentDashCooldown = 0.0f;
    
public:
    void OnCollect(Entity collector) override;
    void OnActivate() override;
    void OnDeactivate() override;
    void Update(float deltaTime) override;
    
    void PerformDash(Entity owner);
};
```

### Boomerang Suit
```cpp
class BoomerangSuitPowerUp : public PowerUpComponent {
private:
    struct Boomerang {
        Vector2D Position;
        Vector2D Velocity;
        float ReturnTimer;
        bool IsReturning;
    };
    
    std::vector<Boomerang> m_ActiveBoomerangs;
    float m_ThrowSpeed = 12.0f;
    float m_ReturnSpeed = 15.0f;
    float m_MaxDistance = 8.0f;
    int m_MaxBoomerangs = 2;
    float m_Cooldown = 1.0f;
    
public:
    void ThrowBoomerang(const Vector2D& direction);
    void UpdateBoomerangs(float deltaTime);
    void OnHit(Entity target);
};
```

### Clone Cherry
```cpp
class CloneCherryPowerUp : public PowerUpComponent {
private:
    struct Clone {
        Entity CloneEntity;
        float ActionDelay;
        std::queue<PlayerAction> ActionQueue;
    };
    
    std::vector<Clone> m_Clones;
    int m_MaxClones = 3;
    float m_CloneSpacing = 1.0f;
    float m_ActionDelay = 0.2f;
    
public:
    void CreateClone(const Vector2D& position);
    void UpdateClones(float deltaTime);
    void RecordPlayerAction(const PlayerAction& action);
    void OnCloneHit(Entity clone);
};
```

### Fire Flower
```cpp
class FireFlowerPowerUp : public PowerUpComponent {
private:
    struct Fireball {
        Vector2D Position;
        Vector2D Velocity;
        float Lifetime;
        bool Active;
    };
    
    std::vector<Fireball> m_ActiveFireballs;
    float m_FireballSpeed = 10.0f;
    float m_FireRate = 0.8f;
    float m_CurrentCooldown = 0.0f;
    float m_FireballLifetime = 2.0f;
    int m_Damage = 1;
    
public:
    void ShootFireball(const Vector2D& direction);
    void UpdateFireballs(float deltaTime);
    void OnFireballHit(Entity target);
};
```

### Propeller Box
```cpp
class PropellerBoxPowerUp : public PowerUpComponent {
private:
    float m_FloatDuration = 2.0f;
    float m_CurrentFloatTime = 0.0f;
    float m_BoostSpeed = 14.0f;
    float m_HoverFallSpeed = 2.0f;
    float m_BoostCooldown = 1.5f;
    float m_CurrentBoostCooldown = 0.0f;
    float m_AirControlMultiplier = 1.2f;
    bool m_IsFloating = false;
    
public:
    void StartFloat();
    void UpdateFloat(float deltaTime);
    void ApplyBoost();
    void ModifyAirControl(PhysicsComponent* physics);
};
```

## 3. Power-Up Management

### Power-Up Manager
```cpp
class PowerUpManager : public System {
private:
    struct ActivePowerUp {
        PowerUpComponent::Type Type;
        float RemainingTime;
        int Stacks;
    };
    
    std::unordered_map<Entity, std::vector<ActivePowerUp>> m_ActivePowerUps;
    PowerUpFactory m_Factory;
    
public:
    void AddPowerUp(Entity entity, PowerUpComponent::Type type);
    void RemovePowerUp(Entity entity, PowerUpComponent::Type type);
    void UpdatePowerUps(float deltaTime);
    
    bool HasPowerUp(Entity entity, PowerUpComponent::Type type) const;
    int GetPowerUpStacks(Entity entity, PowerUpComponent::Type type) const;
};
```

## 4. Spawn System

### Power-Up Spawner
```cpp
class PowerUpSpawner : public System {
private:
    struct SpawnPoint {
        Vector2D Position;
        std::vector<PowerUpComponent::Type> PossibleTypes;
        float SpawnChance;
        float RespawnTime;
        float CurrentTime;
    };
    
    std::vector<SpawnPoint> m_SpawnPoints;
    
public:
    void AddSpawnPoint(
        const Vector2D& position,
        const std::vector<PowerUpComponent::Type>& types,
        float chance,
        float respawnTime
    );
    
    void Update(float deltaTime);
    PowerUpComponent::Type SelectRandomPowerUp(const SpawnPoint& point);
};
```

## Implementation Timeline

### Week 1
- Day 1-2: Base power-up system
- Day 3-4: Basic power-ups (Speed Bell, Fire Flower)
- Day 5-7: Power-up management system

### Week 2
- Day 1-2: Advanced power-ups (Boomerang, Clone)
- Day 3-4: Spawn system
- Day 5-7: Testing and balancing

## Performance Targets

### Power-Up System
- Maximum active power-ups: 100
- Update time < 0.5ms
- Memory usage < 50KB
- Spawn calculations < 0.1ms

### Effects Performance
- Particle effects < 0.3ms
- Physics modifications < 0.2ms
- Clone updates < 0.5ms
- Projectile management < 0.4ms

## Testing Strategy

### Unit Tests
- Power-up activation/deactivation
- Duration tracking
- Stacking behavior
- Spawn system
- Effect calculations

### Integration Tests
- Player interaction
- Multiple power-up handling
- Physics integration
- Visual effects
- Sound effects

### Balance Testing
- Power-up effectiveness
- Duration values
- Spawn rates
- Stacking effects
- Cooldown times

## Debug Features

### Power-Up Inspector
```cpp
class PowerUpDebugger {
public:
    void ShowActivePowerUps(Entity entity);
    void ShowSpawnPoints();
    void ShowEffectRadius();
    void ShowCooldowns();
    void ShowStackInfo();
};
```

### Visualization
- Power-up ranges
- Effect durations
- Spawn points
- Cooldown timers
- Stack counters

## Power-Up Data Format (JSON)
```json
{
    "powerups": {
        "speed_bell": {
            "duration": -1,
            "speed_multiplier": 1.5,
            "dash_force": 3.0,
            "dash_cooldown": 1.0,
            "can_stack": false
        },
        "boomerang_suit": {
            "duration": -1,
            "throw_speed": 12.0,
            "return_speed": 15.0,
            "max_distance": 8.0,
            "max_boomerangs": 2,
            "cooldown": 1.0
        },
        "clone_cherry": {
            "duration": -1,
            "max_clones": 3,
            "clone_spacing": 1.0,
            "action_delay": 0.2,
            "can_stack": false
        },
        "fire_flower": {
            "duration": -1,
            "fireball_speed": 10.0,
            "fire_rate": 0.8,
            "fireball_lifetime": 2.0,
            "damage": 1
        },
        "propeller_box": {
            "duration": 10.0,
            "float_duration": 2.0,
            "boost_speed": 14.0,
            "hover_fall_speed": 2.0,
            "boost_cooldown": 1.5,
            "air_control": 1.2
        }
    }
}
```
