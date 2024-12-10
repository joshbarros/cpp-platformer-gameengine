#pragma once

#include <string>

class Debug {
public:
    static void Assert(bool condition, const std::string& message = "", const char* file = __FILE__, int line = __LINE__);
    static void Break();
    
private:
    static void HandleAssertionFailure(const std::string& message, const char* file, int line);
};

#ifdef NDEBUG
    #define ASSERT(condition, message) ((void)0)
    #define DEBUG_BREAK() ((void)0)
#else
    #define ASSERT(condition, message) Debug::Assert(condition, message, __FILE__, __LINE__)
    #define DEBUG_BREAK() Debug::Break()
#endif
