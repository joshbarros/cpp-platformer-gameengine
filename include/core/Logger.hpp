#pragma once

#include <string>
#include <sstream>
#include <iostream>

enum class LogLevel {
    Trace,
    Info,
    Warning,
    Error,
    Critical
};

class Logger {
public:
    static void Init();
    static void Shutdown();
    
    template<typename... Args>
    static void Log(LogLevel level, const std::string& fmt, Args&&... args) {
        std::stringstream ss;
        FormatString(ss, fmt, std::forward<Args>(args)...);
        LogMessage(level, ss.str());
    }
    
    static void Info(const std::string& message) {
        Log(LogLevel::Info, message);
    }
    
    static void Warn(const std::string& message) {
        Log(LogLevel::Warning, message);
    }
    
    static void Error(const std::string& message) {
        Log(LogLevel::Error, message);
    }
    
private:
    static void LogMessage(LogLevel level, const std::string& message);
    static const char* GetLevelString(LogLevel level);
    
    static void FormatString(std::stringstream& ss, const std::string& fmt) {
        ss << fmt;
    }
    
    template<typename T>
    static void FormatString(std::stringstream& ss, const std::string& fmt, T&& value) {
        size_t pos = fmt.find("{}");
        if (pos != std::string::npos) {
            ss << fmt.substr(0, pos) << value << fmt.substr(pos + 2);
        } else {
            ss << fmt;
        }
    }
    
    template<typename T, typename... Args>
    static void FormatString(std::stringstream& ss, const std::string& fmt, T&& value, Args&&... args) {
        size_t pos = fmt.find("{}");
        if (pos != std::string::npos) {
            ss << fmt.substr(0, pos) << value;
            FormatString(ss, fmt.substr(pos + 2), std::forward<Args>(args)...);
        } else {
            ss << fmt;
        }
    }
    
    static bool s_Initialized;
};

// Convenience macros
#define LOG_TRACE(...)    Logger::Log(LogLevel::Trace, __VA_ARGS__)
#define LOG_INFO(...)     Logger::Log(LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(...)     Logger::Log(LogLevel::Warning, __VA_ARGS__)
#define LOG_ERROR(...)    Logger::Log(LogLevel::Error, __VA_ARGS__)
#define LOG_CRITICAL(...) Logger::Log(LogLevel::Critical, __VA_ARGS__)
