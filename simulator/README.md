# XyrisSim

XyrisSim is the reference simulator for XyrisOS. It provides a lightweight environment for developing and testing core operating system concepts without requiring immediate execution on physical hardware.

The simulator is designed to support:

- System initialization
- Memory subsystem testing
- CPU subsystem testing
- Basic command execution
- Automated testing through CTest
- Development and continuous integration workflows

---

## Directory Structure

```text
simulator/
├── include/        # Public header files
├── src/            # Simulator implementation
├── tests/          # Unit and integration tests
├── docs/           # Simulator documentation
└── CMakeLists.txt  # Simulator build configuration