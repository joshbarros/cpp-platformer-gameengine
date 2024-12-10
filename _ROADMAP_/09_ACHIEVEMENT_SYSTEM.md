# Phase 9: Achievement System - Detailed Design Document

## Overview
A comprehensive achievement system with Steam integration, tracking player progress and accomplishments.

## 1. Achievement Core

### Achievement Manager
```cpp
class AchievementManager {
private:
    struct AchievementData {
        std::string ID;
        std::string Name;
        std::string Description;
        std::string IconPath;
        bool IsHidden;
        bool IsUnlocked;
        float Progress;
        time_t UnlockTime;
    };
    
    std::unordered_map<std::string, AchievementData> m_Achievements;
    bool m_SteamInitialized;
    
public:
    void Initialize();
    void LoadAchievements();
    void Update();
    
    void UnlockAchievement(const std::string& id);
    void SetProgress(const std::string& id, float progress);
    void ResetAchievements();
    
    bool IsAchievementUnlocked(const std::string& id) const;
    float GetProgress(const std::string& id) const;
};
```

### Steam Integration
```cpp
class SteamAchievements {
private:
    bool m_Initialized;
    ISteamUserStats* m_SteamUserStats;
    
public:
    bool Init();
    void RequestStats();
    bool SetAchievement(const char* id);
    bool ClearAchievement(const char* id);
    bool StoreStats();
    
    void OnUserStatsReceived(UserStatsReceived_t* pCallback);
    void OnUserStatsStored(UserStatsStored_t* pCallback);
    void OnAchievementStored(UserAchievementStored_t* pCallback);
};
```

## 2. Achievement Types

### Progress Achievement
```cpp
class ProgressAchievement {
private:
    std::string m_ID;
    float m_CurrentProgress;
    float m_TargetProgress;
    bool m_IsComplete;
    
public:
    void UpdateProgress(float progress);
    void Reset();
    float GetPercentage() const;
    bool IsComplete() const;
};
```

### Conditional Achievement
```cpp
class ConditionalAchievement {
private:
    std::string m_ID;
    std::function<bool()> m_Condition;
    bool m_IsComplete;
    
public:
    void SetCondition(std::function<bool()> condition);
    void Check();
    bool IsComplete() const;
};
```

### Sequential Achievement
```cpp
class SequentialAchievement {
private:
    std::string m_ID;
    std::vector<std::string> m_Steps;
    size_t m_CurrentStep;
    bool m_IsComplete;
    
public:
    void AddStep(const std::string& step);
    void CompleteStep(const std::string& step);
    size_t GetCurrentStep() const;
    bool IsComplete() const;
};
```

## 3. Achievement Categories

### Story Achievements
```cpp
namespace Achievements {
    class StoryProgress {
    public:
        static void OnWorldComplete(int worldIndex);
        static void OnAllLevelsComplete();
        static void OnPerfectRun(int levelIndex);
        static void OnSpeedRun(int levelIndex, float time);
    };
}
```

### Collection Achievements
```cpp
namespace Achievements {
    class Collection {
    private:
        static int s_TotalCoins;
        static int s_SecretAreas;
        static std::set<std::string> s_PowerUps;
        
    public:
        static void OnCoinCollected();
        static void OnSecretFound();
        static void OnPowerUpCollected(const std::string& powerUp);
    };
}
```

### Combat Achievements
```cpp
namespace Achievements {
    class Combat {
    private:
        static int s_EnemiesDefeated;
        static int s_BossesDefeated;
        static float s_ComboDuration;
        static int s_ComboCount;
        
    public:
        static void OnEnemyDefeated();
        static void OnBossDefeated(bool perfectRun);
        static void OnComboPerformed(int count);
    };
}
```

## 4. UI Integration

### Achievement Notification
```cpp
class AchievementNotification {
private:
    struct NotificationData {
        std::string Title;
        std::string Description;
        Texture* Icon;
        float Duration;
        float CurrentTime;
    };
    
    std::queue<NotificationData> m_Queue;
    std::unique_ptr<UIElement> m_NotificationUI;
    
public:
    void ShowNotification(const std::string& achievementId);
    void Update(float deltaTime);
    void Render();
};
```

### Achievement UI
```cpp
class AchievementUI {
private:
    struct AchievementDisplay {
        UIImage* Icon;
        UIText* Title;
        UIText* Description;
        UIProgressBar* Progress;
        bool IsHidden;
    };
    
    std::vector<AchievementDisplay> m_Displays;
    
public:
    void Initialize();
    void UpdateDisplay();
    void ShowHidden(bool show);
    void SortByCompletion();
    void FilterByCategory(const std::string& category);
};
```

## 5. Statistics Tracking

### Stats Manager
```cpp
class StatsManager {
private:
    struct PlayerStats {
        int TotalPlayTime;
        int LevelsCompleted;
        int CoinsCollected;
        int EnemiesDefeated;
        int PowerUpsCollected;
        int DeathCount;
        std::map<std::string, int> PowerUpUsage;
    };
    
    PlayerStats m_Stats;
    
public:
    void UpdateStat(const std::string& stat, int value);
    void IncrementStat(const std::string& stat);
    int GetStat(const std::string& stat) const;
    void SaveStats();
    void LoadStats();
};
```

## Implementation Timeline

### Week 1
- Day 1-2: Core achievement system
- Day 3-4: Steam integration
- Day 5-7: Achievement types implementation

### Week 2
- Day 1-2: UI notifications
- Day 3-4: Statistics tracking
- Day 5-7: Testing and balancing

## Performance Targets

### Achievement System
- Achievement check time < 0.1ms
- Memory usage < 10MB
- Save time < 100ms
- Notification delay < 16ms

### Steam Integration
- API response time < 500ms
- Sync time < 1s
- Cache size < 5MB

## Testing Strategy

### Unit Tests
- Achievement unlocking
- Progress tracking
- Condition checking
- Stats recording
- Save/load operations

### Integration Tests
- Steam API integration
- UI notification system
- Statistics tracking
- Achievement persistence
- Multiple achievement triggers

### User Experience Tests
- Notification timing
- UI responsiveness
- Achievement clarity
- Progress feedback
- Steam sync reliability

## Debug Features

### Achievement Debugger
```cpp
class AchievementDebugger {
public:
    void ShowAchievements();
    void ShowProgress();
    void ShowStats();
    void UnlockAll();
    void ResetAll();
    void SimulateConditions();
};
```

### Visualization
- Achievement progress
- Unlock conditions
- Statistics graphs
- Notification queue
- Steam sync status

## Achievement Data Format (JSON)
```json
{
    "achievements": {
        "WORLD_1_COMPLETE": {
            "name": "Just Getting Started",
            "description": "Complete World 1",
            "icon": "achievements/world1.png",
            "hidden": false,
            "steam_api_name": "ACH_WORLD1"
        },
        "COIN_COLLECTOR": {
            "name": "Treasure Hunter",
            "description": "Collect 100 coins",
            "icon": "achievements/coins.png",
            "hidden": false,
            "steam_api_name": "ACH_COINS_100",
            "progress": {
                "max": 100,
                "increment": 1
            }
        },
        "PERFECT_BOSS": {
            "name": "Untouchable",
            "description": "Defeat a boss without taking damage",
            "icon": "achievements/perfect.png",
            "hidden": true,
            "steam_api_name": "ACH_PERFECT_BOSS"
        }
    },
    "statistics": {
        "playtime": {
            "display_name": "Total Play Time",
            "format": "hours",
            "steam_api_name": "STAT_PLAYTIME"
        },
        "enemies_defeated": {
            "display_name": "Enemies Defeated",
            "format": "number",
            "steam_api_name": "STAT_ENEMIES"
        },
        "death_count": {
            "display_name": "Total Deaths",
            "format": "number",
            "steam_api_name": "STAT_DEATHS"
        }
    },
    "ui": {
        "notification": {
            "duration": 3.0,
            "position": {"x": 0.8, "y": 0.1},
            "animation": {
                "type": "slide",
                "duration": 0.3
            }
        },
        "menu": {
            "grid_size": {"x": 4, "y": 3},
            "show_hidden": false,
            "sort_by": "completion"
        }
    }
}
```
