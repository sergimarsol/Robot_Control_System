# MSP430FR2355 Robot Control System

This repository contains the complete development of a robotic control system based on the MSP430FR2355 microcontroller. The project encompasses hardware design, embedded software development, real-time control implementation, and autonomous behavior strategies. All stages—from conceptual engineering to full system deployment—were carried out by a single author.

## Project Overview

The system enables robot control through multiple operational modes, leveraging a modular driver-based firmware architecture. It integrates input devices, actuators, and display components to provide both manual and autonomous control functionalities. The hardware was designed, assembled, debugged, and calibrated to ensure correct interaction among all peripherals.

### Control Modes
- **Joystick Mode**: Manual navigation using an analog joystick.
- **Light-Following Mode**: Robot movement directed toward the strongest detected light using LDR readings.
- **Line-Following Mode**: Ground sensor-based autonomous navigation via line detection.
- **Main Menu Interface**: LCD-driven user interface navigated using the joystick.

## Hardware Architecture

| Component | Description |
|-----------|-------------|
| MSP430FR2355 MCU | Core controller handling I2C, ADC, timers, GPIOs |
| 32.7KHz Crystal | Clock stabilization |
| SBW Connector | Programming and debugging interface |
| LCD 16x2 (I2C) | Displays system information and menu |
| Joystick | Directional input for manual control and navigation |
| LDR Sensors (ADC) | Detect light intensity for autonomous movement |
| Motor Driver (I2C) | Controls wheel motors for robot movement |
| RGB LEDs (I2C) | Status indication |
| ESP-01 Module (UART) | Planned Wi-Fi communication support |

## Software Architecture

The firmware was implemented with a modular structure to maximize maintainability and reusability.

| Module | Purpose |
|--------|---------|
| `main.c` | System initialization and main control loop |
| `timer.h` | Timer configuration, delays, millisecond counter ISR |
| `i2c.h` | I2C communication management and ISR |
| `lcd.h` | LCD initialization and text display functions |
| `leds.h` | RGB LED color control |
| `motors.h` | Motor control for direction and velocity |
| `joystick.h` | Joystick reading and movement triggering |
| `adc.h` | Light sensor acquisition and averaging |
| `sensors.h` | Line detection and autonomous path adjustment |

## Proposed Repository Structure

```
MSP430-Robot-Control-System
├── hardware/
│   ├── schematics.pdf
│   └── layout.pcb
├── firmware/
│   ├── main.c
│   ├── lcd.h
│   ├── i2c.h
│   ├── motors.h
│   ├── joystick.h
│   ├── adc.h
│   ├── sensors.h
│   └── timer.h
├── docs/
│   ├── diagrams/
│   └── SergiMarsol_projecte_MISE.pptx
└── README.md
```

## Development Highlights

| Phase | Achievement |
|-------|------------|
| Concept Engineering | Block diagrams, requirements, planning |
| Hardware Implementation | Complete schematic design, assembly, correction of ADC pin assignments |
| Software Implementation | Driver-based structure enabling scalability |
| Debugging | Systematic validation using modular subsystem tests |
| Optional Expansion | UART-based Wi-Fi communication ready for future phases |

## Conclusion

The project demonstrates an end-to-end realization of an embedded robotic control system, showcasing microcontroller-based hardware integration, real-time firmware development, and autonomous control strategies. It serves as an excellent reference for modular embedded system design using the MSP430 ecosystem.

---

For future developments, planned features include remote control via Wi-Fi, telemetry reporting, and enhanced control algorithms such as PID for smoother autonomous navigation.
