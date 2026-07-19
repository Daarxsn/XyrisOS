# XyrisSim

XyrisSim is the reference simulator for XyrisOS. It provides a controlled environment for developing and testing core operating system components before integration with the target system.

## Structure

simulator/
├── include/        # Header files
├── src/            # Simulator source files
├── docs/           # Simulator documentation
└── CMakeLists.txt  # Build configuration

## Requirements

- CMake 3.20 or newer
- Ninja
- C compiler with C23 support

## Build

From the XyrisOS repository root:

    cmake -S simulator -B simulator/build -G Ninja
    cmake --build simulator/build

## Run the Simulator

After building:

    ./simulator/build/XyrisSim

## Run Tests

    ctest --test-dir simulator/build --output-on-failure

Or run the complete workflow:

    ./scripts/simulator-test.sh

## Components

The simulator currently includes:

- CPU simulation
- Memory simulation
- Display simulation
- System simulation
- Simulator startup and execution flow

## Continuous Integration

Simulator builds and tests are automatically executed using GitHub Actions.

The workflow is located at:

    .github/workflows/simulator-tests.yml

## Documentation

Additional documentation is available in:

- docs/architecture.md
- docs/roadmap.md
- docs/test_plan.md
