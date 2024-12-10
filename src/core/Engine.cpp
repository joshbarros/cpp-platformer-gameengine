#include "core/Engine.hpp"
#include "core/Logger.hpp"
#include <GLFW/glfw3.h>

Engine::Engine()
    : m_Running(false)
    , m_Accumulator(0.0)
    , m_PlayerX(100.0f)
    , m_PlayerY(100.0f)
    , m_PlayerSpeed(300.0f)
    , m_IsJumping(false)
    , m_JumpForce(500.0f)
    , m_Gravity(980.0f)
    , m_VerticalVelocity(0.0f) {
    // Initialize logger
    Logger::Init();
}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Init() {
    LOG_INFO("Initializing engine...");
    
    // Create window
    m_Window = std::make_unique<Window>();
    if (!m_Window->Init({
        .Title = "Platform Game",
        .Width = 1280,
        .Height = 720,
        .VSync = true,
        .Fullscreen = false
    })) {
        LOG_ERROR("Failed to create window");
        return false;
    }
    
    // Create timer
    m_Timer = std::make_unique<Timer>();
    
    // Create input system
    m_Input = std::make_unique<Input>(m_Window.get());
    
    // Set up default input mappings
    m_Input->MapAction("Jump", { GLFW_KEY_SPACE });
    m_Input->MapAction("MoveLeft", { GLFW_KEY_A, GLFW_KEY_LEFT });
    m_Input->MapAction("MoveRight", { GLFW_KEY_D, GLFW_KEY_RIGHT });
    m_Input->MapAction("Crouch", { GLFW_KEY_S, GLFW_KEY_DOWN });
    m_Input->MapAction("Run", { GLFW_KEY_LEFT_SHIFT });
    
    LOG_INFO("Input system initialized with the following controls:");
    LOG_INFO("- SPACE: Jump");
    LOG_INFO("- A/LEFT: Move left");
    LOG_INFO("- D/RIGHT: Move right");
    LOG_INFO("- S/DOWN: Crouch");
    LOG_INFO("- LEFT SHIFT: Run");
    LOG_INFO("- ESC: Exit");
    
    m_Running = true;
    return true;
}

void Engine::Run() {
    LOG_INFO("Starting game loop...");
    
    while (m_Running) {
        m_Timer->Update();
        double deltaTime = m_Timer->GetDeltaTime();
        
        // Update input
        m_Input->Update();
        
        // Handle escape key to close window
        if (m_Input->IsKeyPressed(GLFW_KEY_ESCAPE)) {
            m_Running = false;
        }
        
        // Fixed timestep update
        m_Accumulator += deltaTime;
        while (m_Accumulator >= FIXED_TIME_STEP) {
            FixedUpdate(FIXED_TIME_STEP);
            m_Accumulator -= FIXED_TIME_STEP;
        }
        
        // Variable timestep update
        Update(deltaTime);
        
        // Render
        Render();
        
        // Update window
        m_Window->Update();
        
        // Check if window should close
        if (m_Window->ShouldClose()) {
            m_Running = false;
        }
    }
}

void Engine::Update(float deltaTime) {
    bool inputChanged = false;
    
    // Handle horizontal movement
    if (m_Input->IsActionActive("MoveLeft")) {
        m_PlayerX -= m_PlayerSpeed * deltaTime;
        LOG_INFO(">>> Moving LEFT  | Position: X={:.1f}, Y={:.1f}", m_PlayerX, m_PlayerY);
        inputChanged = true;
    }
    else if (m_Input->IsActionActive("MoveRight")) {
        m_PlayerX += m_PlayerSpeed * deltaTime;
        LOG_INFO(">>> Moving RIGHT | Position: X={:.1f}, Y={:.1f}", m_PlayerX, m_PlayerY);
        inputChanged = true;
    }
    
    // Handle jumping
    if (m_Input->IsActionActive("Jump") && !m_IsJumping) {
        m_IsJumping = true;
        m_VerticalVelocity = m_JumpForce;
        LOG_INFO(">>> JUMP started! Initial velocity: {:.1f}", m_VerticalVelocity);
        inputChanged = true;
    }
    
    // Apply gravity
    m_VerticalVelocity -= m_Gravity * deltaTime;
    m_PlayerY += m_VerticalVelocity * deltaTime;
    
    // Simple ground collision
    if (m_PlayerY <= 100.0f) {
        m_PlayerY = 100.0f;
        m_VerticalVelocity = 0.0f;
        if (m_IsJumping) {
            LOG_INFO(">>> JUMP ended | Landing position: X={:.1f}, Y={:.1f}", m_PlayerX, m_PlayerY);
            m_IsJumping = false;
        }
    }
    
    // Handle crouching
    if (m_Input->IsActionActive("Crouch")) {
        LOG_INFO(">>> CROUCHING | Position: X={:.1f}, Y={:.1f}", m_PlayerX, m_PlayerY);
        inputChanged = true;
    }
    
    // Handle running
    if (m_Input->IsActionActive("Run")) {
        LOG_INFO(">>> RUNNING | Position: X={:.1f}, Y={:.1f}", m_PlayerX, m_PlayerY);
        inputChanged = true;
    }
    
    // Get mouse position (only log if significant movement and no keyboard input)
    if (!inputChanged) {
        double mouseX, mouseY;
        m_Input->GetMousePosition(mouseX, mouseY);
        
        static double lastMouseX = mouseX;
        static double lastMouseY = mouseY;
        if (abs(mouseX - lastMouseX) > 10.0 || abs(mouseY - lastMouseY) > 10.0) {
            LOG_INFO("Mouse position: X={:.1f}, Y={:.1f}", mouseX, mouseY);
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
    }
}

void Engine::FixedUpdate(float fixedDeltaTime) {
    // Fixed timestep physics update will go here
}

void Engine::Render() {
    m_Window->Clear();
    m_Window->SwapBuffers();
}

void Engine::Shutdown() {
    LOG_INFO("Shutting down engine...");
    m_Input.reset();
    m_Window.reset();
    Logger::Shutdown();
}
