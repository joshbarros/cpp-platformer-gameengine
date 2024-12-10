#include "core/Timer.hpp"

Timer::Timer()
    : m_LastUpdate(Clock::now())
    , m_DeltaTime(0.0)
    , m_TotalTime(0.0) {
}

void Timer::Update() {
    TimePoint now = Clock::now();
    std::chrono::duration<double> duration = now - m_LastUpdate;
    m_DeltaTime = duration.count();
    m_TotalTime += m_DeltaTime;
    m_LastUpdate = now;
}

void Timer::Reset() {
    m_LastUpdate = Clock::now();
    m_DeltaTime = 0.0;
    m_TotalTime = 0.0;
}
