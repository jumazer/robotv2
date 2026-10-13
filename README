# STM32 Mobile Robot Controller (V2)

## Overview

This project is a four-motor mobile robot controlled by an **STM32 Nucleo-F411RE** using register-level C firmware. Commands arrive over a Bluetooth serial connection, are parsed into throttle and turning inputs, and control two motor groups (left and right) through PWM and GPIO direction signals.

V2 also integrates an **HC-SR04 ultrasonic distance sensor**. The firmware measures the echo pulse duration and stops the motors when it detects an obstacle within a configured distance. This is an obstacle-detection stop, **not autonomous navigation or PID control**.

The project builds on the original graduate embedded-systems robot and continues as a personal embedded robotics project.

## Demo and project history

- [Driving demo — original V1 robot](https://www.youtube.com/watch?v=r0X5UvUqSxo) **(V1 footage; does not demonstrate V2's ultrasonic sensor)**
- [Original V1 repository and documentation](https://github.com/jumazer/robot-final-project)
- V2 photos and a current driving demonstration will be added later.

## Features

- STM32F411RE firmware written in C using **CMSIS register access** rather than STM32 HAL
- Interrupt-driven Bluetooth serial reception through **USART6**, with circular FIFO buffering
- Separate debug serial interface through **USART2**
- PWM motor-speed control using **TIM3 and TIM2**, with GPIO direction control
- Four DC gear motors driven as **two left/right groups** for differential steering
- Fixed movement commands and joystick-style throttle/turn commands
- **HC-SR04** ultrasonic ranging using a timer-based microsecond delay and echo pulse timing
- Obstacle-triggered stop and forward-movement inhibit with separate stop and clear thresholds
- Modular source files for peripheral initialization, communication, command parsing, motor control, and sensing

## Hardware

- STM32 Nucleo-F411RE development board (STM32F411RE)
- HC-05 Bluetooth serial module
- TB6612FNG motor driver
- Four TT-style DC gear motors, grouped by left/right side
- HC-SR04 ultrasonic distance sensor
- Separate motor power source and USB connection for development/debugging

**Motor configuration:** The four motors are not independently speed-controlled. The two motors on each side act as one motor group, with one PWM speed command per side.

### STM32 pin mapping

The following assignments reflect the V2 firmware:

| Function | STM32 pin | Peripheral / purpose |
| --- | --- | --- |
| Debug TX / RX | PA2 / PA3 | USART2 |
| Bluetooth TX / RX | PA11 / PA12 | USART6 |
| Motor PWM output 1 | PA6 | TIM3_CH1 |
| Motor PWM output 2 | PB10 | TIM2_CH3 |
| Motor driver standby | PC7 | STBY |
| Motor direction signals | PA9, PA8, PB6, PA7 | AIN1, AIN2, BIN1, BIN2 |
| Ultrasonic trigger | PB9 | TRIG |
| Ultrasonic echo | PB8 | ECHO |

> **Electrical note:** Use a common ground between the STM32, motor driver, sensor, and external supply. Ensure signals connected to STM32 GPIOs are at safe logic levels; the HC-SR04 ECHO line may require level shifting depending on the sensor module and wiring.

## Software architecture

| File | Responsibility |
| --- | --- |
| `Src/main.c` | Initializes peripherals, processes Bluetooth commands, and applies obstacle-stop logic |
| `Src/board.c` | Configures GPIO for the motor driver and ultrasonic sensor |
| `Src/usart.c` | Configures USART2/USART6, interrupt handlers, and receive buffers |
| `Src/cbfifo.c` | Implements circular receive buffers |
| `Src/command_processor.c` | Parses fixed commands and joystick-style inputs |
| `Src/motor_control.c` | Converts throttle/turn requests into direction and PWM values |
| `Src/pwm.c` | Configures TIM3_CH1 and TIM2_CH3 PWM outputs |
| `Src/echo_processor.c` | Triggers ultrasonic measurements, times echo pulses, and estimates distance |
| `Src/timers.c` | Provides SysTick-based timing and TIM5 microsecond timing |

## Bluetooth control

Commands are received through the Bluetooth serial interface and processed as newline-terminated messages.

### Fixed commands

| Command | Action |
| --- | --- |
| `w` | Forward |
| `s` | Reverse |
| `a` | Turn left |
| `d` | Turn right |
| `q` | Stop |

### Joystick-style commands

The parser also accepts six-character commands such as `f50r20`:

- `f50`: Forward throttle of 50 (or `b` for reverse)
- `r20`: Right turn input of 20 (or `l` for left)

The firmware mixes throttle and turning inputs to produce left- and right-side PWM commands, with output speed clamping. These values are controller units, not calibrated physical speeds.

## Ultrasonic obstacle-stop behavior

V2 uses the HC-SR04 trigger/echo interface to estimate distance from the echo pulse duration. In the current firmware:

1. An ultrasonic measurement is requested periodically, approximately every 100 ms.
2. A valid measured distance of **30 cm or less** causes the motors to stop and blocks forward commands.
3. Forward commands remain blocked until a valid reading reaches **35 cm or more**.
4. Clearing the blocked state does **not** automatically resume movement; a new movement command is required.

The separate stop and clear thresholds provide hysteresis to reduce repeated switching near the limit. The implementation does not autonomously steer around obstacles.

## Build and flash

The project is structured for **STM32CubeIDE** and targets the STM32F411RE.

1. Open or import the project in STM32CubeIDE.
2. Connect the Nucleo-F411RE board for programming and debugging.
3. Build the project and flash it to the board.
4. Connect the Bluetooth module and motor/sensor hardware according to the V2 pin assignments.
5. Send newline-terminated commands over the Bluetooth serial link. Debug output is available through USART2.

## Testing and current limitations

- The `Test/` directory contains FIFO tests and command-processing test code. **Some command tests refer to an older command API and need to be updated for V2.** Do not treat them as evidence of a passing V2 test suite.
- Ultrasonic echo timeouts currently return without a new distance reading; a sensor-failure stop policy has not been implemented.
- The Bluetooth communication watchdog described in V1 has **not yet been carried over to V2**.
- PID motor speed control, encoder feedback, and autonomous navigation are **not implemented**.

## Planned improvements

- Implement and validate PID-based motor control with appropriate feedback sensors
- Restore a communication-loss safety stop and improve sensor-failure handling
- Update and run the V2 command-processing tests
- Add V2 photographs, wiring diagrams, and an updated driving demo

## Background

This robot began as a graduate project for **Principles of Embedded Systems** at the University of Colorado Boulder and has since been extended. The [V1 repository](https://github.com/jumazer/robot-final-project) preserves the original project documentation and demonstration.