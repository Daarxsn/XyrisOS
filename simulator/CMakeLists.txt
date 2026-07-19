cmake_minimum_required(VERSION 3.20)

project(XyrisSim C)

set(CMAKE_C_STANDARD 23)

include_directories(include)

add_executable(XyrisSim
    src/main.c
    src/simulator.c
    src/cpu.c
    src/memory.c
    src/display.c
    src/system.c
)

add_executable(test_runner
    ../tests/test_runner.c
    ../tests/test_system.c
    ../tests/test_memory.c
    ../tests/test_cpu.c
    src/system.c
    src/memory.c
    src/cpu.c
)

target_include_directories(test_runner PRIVATE
    include
)

enable_testing()

add_test(
    NAME XyrisOS_System_Tests
    COMMAND test_runner
)