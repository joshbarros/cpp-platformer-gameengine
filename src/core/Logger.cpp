#include "core/Logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

bool Logger::s_Initialized = false;

void Logger::Init() {
    s_Initialized = true;
}

void Logger::Shutdown() {
    s_Initialized = false;
}

void Logger::LogMessage(LogLevel level, const std::string& message) {
    if (!s_Initialized) return;
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    // Format timestamp
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    // Output log message
    std::cout << "[" << ss.str() << "] [" << GetLevelString(level) << "] " << message << std::endl;
}

const char* Logger::GetLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARN";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}
