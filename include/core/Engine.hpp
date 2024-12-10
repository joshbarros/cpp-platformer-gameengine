#pragma once

#include <memory>
#include "Window.hpp"
#include "Timer.hpp"
#include "Input.hpp"

class Engine {
public:
    Engine();
    ~Engine();
    
    // Delete copy constructor and assignment operator
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    
    bool Init();
    void Run();
    void Shutdown();
    
private:
    void Update(float deltaTime);
    void FixedUpdate(float fixedDeltaTime);
    void Render();
    
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Timer> m_Timer;
    std::unique_ptr<Input> m_Input;
    bool m_Running;
    
    // Fixed timestep variables
    static constexpr double FIXED_TIME_STEP = 1.0 / 60.0;
    double m_Accumulator;
    
    // Player movement
    float m_PlayerX;
    float m_PlayerY;
    float m_PlayerSpeed;
    bool m_IsJumping;
    float m_JumpForce;
    float m_Gravity;
    float m_VerticalVelocity;
};
