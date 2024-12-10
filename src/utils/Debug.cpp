#include "utils/Debug.hpp"
#include "core/Logger.hpp"
#include <csignal>

void Debug::Assert(bool condition, const std::string& message, const char* file, int line) {
    if (!condition) {
        HandleAssertionFailure(message, file, line);
    }
}

void Debug::Break() {
    #ifdef _MSC_VER
        __debugbreak();
    #else
        std::raise(SIGTRAP);
    #endif
}

void Debug::HandleAssertionFailure(const std::string& message, const char* file, int line) {
    std::string assertMessage = std::string("Assertion failed at ") +
        file + ":" + std::to_string(line);
    
    if (!message.empty()) {
        assertMessage += "\nMessage: " + message;
    }
    
    LOG_CRITICAL(assertMessage);
    Break();
}
