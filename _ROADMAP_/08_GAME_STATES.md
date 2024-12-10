# Phase 8: Game States System - Detailed Design Document

## Overview
A comprehensive state management system for handling different game states, menus, and transitions.

## 1. State Management System

### State Interface
```cpp
class State {
public:
    virtual ~State() = default;
    
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual void OnPause() = 0;
    virtual void OnResume() = 0;
    
    virtual void Update(float deltaTime) = 0;
    virtual void FixedUpdate(float fixedDeltaTime) = 0;
    virtual void Render() = 0;
    
    virtual void HandleInput(const InputEvent& event) = 0;
};
```

### State Manager
```cpp
class StateManager {
private:
    std::stack<std::unique_ptr<State>> m_StateStack;
    std::queue<std::function<void()>> m_PendingChanges;
    bool m_IsUpdating;
    
public:
    template<typename T, typename... Args>
    void PushState(Args&&... args);
    
    void PopState();
    void ClearStates();
    
    void Update(float deltaTime);
    void FixedUpdate(float fixedDeltaTime);
    void Render();
    
    State* GetCurrentState();
    bool IsEmpty() const;
    
private:
    void ApplyPendingChanges();
};
```

## 2. Game States

### Main Menu State
```cpp
class MainMenuState : public State {
private:
    struct MenuItem {
        std::string Text;
        std::function<void()> Action;
        Vector2D Position;
        bool Selected;
    };
    
    std::vector<MenuItem> m_MenuItems;
    int m_SelectedIndex;
    UIRenderer m_UIRenderer;
    
public:
    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render() override;
    void HandleInput(const InputEvent& event) override;
    
private:
    void InitializeMenuItems();
    void UpdateSelection(int direction);
    void ExecuteSelectedItem();
};
```

### Level Selection State
```cpp
class LevelSelectionState : public State {
private:
    struct LevelData {
        std::string Name;
        std::string Thumbnail;
        bool Unlocked;
        int StarsEarned;
        float BestTime;
    };
    
    std::vector<LevelData> m_Levels;
    int m_CurrentPage;
    int m_SelectedLevel;
    
public:
    void LoadLevelData();
    void UpdateLevelGrid();
    void SelectLevel(int index);
    void StartSelectedLevel();
};
```

### Gameplay State
```cpp
class GameplayState : public State {
private:
    Scene* m_GameScene;
    PhysicsWorld* m_PhysicsWorld;
    Camera* m_Camera;
    Entity m_Player;
    
    bool m_Paused;
    float m_GameTime;
    
public:
    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void FixedUpdate(float fixedDeltaTime) override;
    
    void PauseGame();
    void ResumeGame();
    void RestartLevel();
    void ExitToMenu();
};
```

### Pause Menu State
```cpp
class PauseMenuState : public State {
private:
    std::unique_ptr<UICanvas> m_PauseMenu;
    GameplayState* m_GameplayState;
    
public:
    void OnEnter() override {
        m_GameplayState->PauseGame();
        InitializePauseMenu();
    }
    
    void OnExit() override {
        m_GameplayState->ResumeGame();
    }
    
private:
    void InitializePauseMenu();
    void HandleMenuSelection(const std::string& option);
};
```

## 3. UI System

### UI Canvas
```cpp
class UICanvas {
private:
    std::vector<std::unique_ptr<UIElement>> m_Elements;
    bool m_Visible;
    int m_ZOrder;
    
public:
    void AddElement(std::unique_ptr<UIElement> element);
    void RemoveElement(UIElement* element);
    void Update(float deltaTime);
    void Render();
    
    void SetVisible(bool visible);
    void SetZOrder(int order);
};
```

### UI Elements
```cpp
class UIElement {
protected:
    Rectangle m_Bounds;
    Vector2D m_Position;
    Vector2D m_Scale;
    float m_Rotation;
    bool m_Visible;
    
public:
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual bool HandleInput(const InputEvent& event) = 0;
    
    void SetPosition(const Vector2D& position);
    void SetScale(const Vector2D& scale);
    void SetRotation(float rotation);
    void SetVisible(bool visible);
};

class Button : public UIElement {
private:
    std::string m_Text;
    Texture* m_NormalTexture;
    Texture* m_HoverTexture;
    Texture* m_PressedTexture;
    std::function<void()> m_OnClick;
    
public:
    void SetTextures(
        Texture* normal,
        Texture* hover,
        Texture* pressed
    );
    
    void SetOnClick(std::function<void()> callback);
    bool HandleInput(const InputEvent& event) override;
};
```

## 4. State Transitions

### Transition System
```cpp
class TransitionSystem {
public:
    enum class Type {
        Fade,
        Slide,
        Zoom,
        Pixelate
    };
    
private:
    Type m_CurrentType;
    float m_Duration;
    float m_Progress;
    bool m_IsTransitioning;
    std::function<void()> m_OnComplete;
    
public:
    void StartTransition(
        Type type,
        float duration,
        std::function<void()> onComplete
    );
    
    void Update(float deltaTime);
    void Render();
    bool IsTransitioning() const;
};
```

## 5. Save System

### Save Manager
```cpp
class SaveManager {
private:
    struct SaveData {
        std::vector<bool> UnlockedLevels;
        std::vector<int> LevelStars;
        std::vector<float> BestTimes;
        std::map<std::string, int> Achievements;
        AudioSettings AudioConfig;
        InputSettings InputConfig;
    };
    
    SaveData m_CurrentSave;
    std::string m_SavePath;
    
public:
    void LoadGame();
    void SaveGame();
    void ResetProgress();
    
    void UnlockLevel(int levelIndex);
    void SetLevelStars(int levelIndex, int stars);
    void SetBestTime(int levelIndex, float time);
};
```

## Implementation Timeline

### Week 1
- Day 1-2: State management system
- Day 3-4: Basic game states
- Day 5-7: UI system implementation

### Week 2
- Day 1-2: State transitions
- Day 3-4: Save system
- Day 5-7: Testing and polish

## Performance Targets

### State Management
- State switch time < 100ms
- Memory overhead < 1MB per state
- UI render time < 2ms
- Input latency < 16ms

### Save System
- Save file size < 1MB
- Load time < 500ms
- Save time < 200ms

## Testing Strategy

### Unit Tests
- State transitions
- UI element functionality
- Save/load operations
- Input handling
- Menu navigation

### Integration Tests
- State stack management
- UI system integration
- Save system reliability
- Transition system
- Audio integration

### User Experience Tests
- Menu responsiveness
- Transition smoothness
- Save reliability
- Input feedback
- Visual consistency

## Debug Features

### State Debugger
```cpp
class StateDebugger {
public:
    void ShowStateStack();
    void ShowUIElements();
    void ShowTransitions();
    void ShowSaveData();
    void ShowInputState();
};
```

### Visualization
- State hierarchy
- UI boundaries
- Input areas
- Transition progress
- Save data structure

## State Configuration (JSON)
```json
{
    "states": {
        "main_menu": {
            "background": "textures/menu_bg.png",
            "music": "music/menu.ogg",
            "menu_items": [
                {
                    "text": "Play Game",
                    "position": {"x": 0.5, "y": 0.4},
                    "action": "start_game"
                },
                {
                    "text": "Options",
                    "position": {"x": 0.5, "y": 0.5},
                    "action": "show_options"
                },
                {
                    "text": "Exit",
                    "position": {"x": 0.5, "y": 0.6},
                    "action": "exit_game"
                }
            ]
        },
        "level_select": {
            "grid_size": {"x": 4, "y": 3},
            "thumbnail_size": {"x": 200, "y": 150},
            "spacing": {"x": 20, "y": 20}
        },
        "gameplay": {
            "pause_enabled": true,
            "background_parallax": true,
            "camera_follow": true
        }
    },
    "transitions": {
        "menu_to_game": {
            "type": "fade",
            "duration": 0.5,
            "color": {"r": 0, "g": 0, "b": 0, "a": 1}
        },
        "level_to_level": {
            "type": "slide",
            "duration": 0.3,
            "direction": "left"
        }
    },
    "ui": {
        "theme": {
            "font": "fonts/main.ttf",
            "font_size": 24,
            "normal_color": {"r": 1, "g": 1, "b": 1, "a": 1},
            "hover_color": {"r": 0.8, "g": 0.8, "b": 1, "a": 1},
            "selected_color": {"r": 1, "g": 0.8, "b": 0.2, "a": 1}
        }
    }
}
```
