# XyrisSim Architecture

## Purpose

XyrisSim is the reference simulator for XyrisOS.

Its purpose is to simulate and test core operating system components before they are integrated into the actual operating system.

The simulator provides a lightweight environment for validating system initialization, component state, kernel boot behavior, reset behavior, and command execution.

---

## Simulator Components

XyrisSim is divided into several simulated components:

### System

The system module maintains the overall state of the simulator.

It tracks:

- CPU status
- Memory status
- Display status
- Kernel status
- Available RAM

### CPU

The CPU module simulates processor initialization and maintains the CPU online state.

### Memory

The memory module simulates memory initialization and maintains the configured RAM state.

### Display

The display module simulates display initialization and maintains the display readiness state.

### Simulator Core

The simulator core controls:

- Simulator initialization
- Kernel boot simulation
- System reset
- Command execution

---

## Initialization Flow

The simulator follows this initialization sequence:

```text
Start XyrisSim
      |
      v
Initialize System State
      |
      v
Initialize CPU
      |
      v
Initialize Memory
      |
      v
Initialize Display
      |
      v
System Ready