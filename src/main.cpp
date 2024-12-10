#include "core/Engine.hpp"
#include "core/Logger.hpp"

int main(int argc, char* argv[]) {
    try {
        // Initialize logger
        Logger::Init();
        LOG_INFO("Starting PlatformerEngine...");
        
        // Create and initialize engine
        Engine engine;
        if (!engine.Init()) {
            LOG_ERROR("Failed to initialize engine");
            return -1;
        }
        
        // Run the engine
        engine.Run();
        
        // Cleanup
        engine.Shutdown();
        Logger::Shutdown();
        
        return 0;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Unhandled exception: {}", e.what());
        return -1;
    }
}
