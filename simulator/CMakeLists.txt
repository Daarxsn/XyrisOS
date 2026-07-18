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

add_executable(test_system
    ../tests/test_system.c
    src/system.c
)

target_include_directories(test_system PRIVATE
    include
)