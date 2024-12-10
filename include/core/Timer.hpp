#pragma once

#include <chrono>

class Timer {
public:
    Timer();
    
    void Update();
    void Reset();
    
    double GetDeltaTime() const { return m_DeltaTime; }
    double GetTotalTime() const { return m_TotalTime; }
    
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
    TimePoint m_LastUpdate;
    double m_DeltaTime;
    double m_TotalTime;
};
