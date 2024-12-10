include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
    core/TimerTests.cpp
    core/WindowTests.cpp
)

add_executable(UnitTests ${TEST_SOURCES})

target_link_libraries(UnitTests
    PRIVATE
        gtest_main
        ${PROJECT_NAME}Lib
)

include(GoogleTest)
gtest_discover_tests(UnitTests)
