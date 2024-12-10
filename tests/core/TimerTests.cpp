#include <gtest/gtest.h>
#include "core/Timer.hpp"
#include <thread>
#include <chrono>

TEST(TimerTests, InitialState) {
    Timer timer;
    EXPECT_DOUBLE_EQ(timer.GetDeltaTime(), 0.0);
    EXPECT_DOUBLE_EQ(timer.GetTotalTime(), 0.0);
}

TEST(TimerTests, Update) {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.Update();
    
    EXPECT_GT(timer.GetDeltaTime(), 0.0);
    EXPECT_GT(timer.GetTotalTime(), 0.0);
}

TEST(TimerTests, Reset) {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.Update();
    timer.Reset();
    
    EXPECT_DOUBLE_EQ(timer.GetDeltaTime(), 0.0);
    EXPECT_DOUBLE_EQ(timer.GetTotalTime(), 0.0);
}
