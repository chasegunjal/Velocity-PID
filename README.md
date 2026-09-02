4-Wheel Mecanum Robot — PID Motor Control & Motion Control

Overview

This repository documents the development of a four-wheel mecanum-drive mobile robot, with a focus on closed-loop motor speed control, position control, motor characterization, wheel synchronization, inverse kinematics, and wireless joystick-based motion control.

The project was developed progressively, starting from basic motor control and encoder feedback and advancing toward a complete motion-control architecture in which the desired robot velocity is converted into individual wheel speeds and regulated using feedback from wheel encoders.

The repository therefore represents not just a final robot-control program, but the control-system development and experimentation process used to build the system.

Aim

To develop a reliable closed-loop control system for a four-wheel mecanum robot capable of:

Controlling individual DC motor speed using encoder feedback.

Regulating motor speed using P, PI, and PID controllers.

Characterizing the motor response to determine feedforward parameters.

Maintaining similar speeds across all four wheels.

Converting robot-level linear and angular velocity commands into individual wheel RPMs using inverse kinematics.

Controlling the robot wirelessly using a PS5 controller.

Progressively improving motion response through feedback, filtering, feedforward, ramping, and wheel synchronization.

Hardware

The software in this repository is designed around a four-wheel mecanum robot using an ESP32-class microcontroller.

Main Hardware

Component

Purpose

ESP32

Main microcontroller and real-time control

4 × DC geared motors

Drive the four mecanum wheels

4 × Quadrature encoders

Measure individual wheel/motor rotation

Motor driver

Drives the DC motors using PWM and direction signals

4 × Mecanum wheels

Enables omnidirectional motion

PS5 controller

Wireless user input for robot motion

External power supply/battery

Supplies power to motors and electronics

Encoder Interface

Each motor is associated with two encoder signals:

Encoder A

Encoder B

The present control implementation primarily uses Encoder A rising-edge interrupts to calculate wheel speed. Encoder B is configured in the hardware interface but is not currently used for direction decoding in the presented control logic.

The project uses approximately 850 pulses per revolution (PPR) as the encoder parameter in the multi-motor controller.

Robot Configuration

The robot uses four independently driven mecanum wheels.

Approximate parameters used in the controller:

Wheel radius  = 0.05 m
Robot radius  = 0.18 m
Encoder PPR   = 850
Maximum RPM   = 200

The four motors are independently controlled through PWM outputs and associated direction outputs.

The controller maintains separate variables for:

Desired RPM

Measured RPM

Filtered RPM

PID integral

Previous error

PWM output

This allows each wheel to operate as an independent closed-loop control system while also participating in the overall robot motion-control system.

Control-System Development

The repository contains several stages of development.

1. Motor Characterization

File: motorCharacterization

Before designing the final controller, the motor response was experimentally characterized.

The program applies different PWM values and measures the resulting steady-state RPM using the encoder.

Example procedure:

PWM → Motor → Encoder → RPM measurement

PWM values are swept across a range, allowing the relationship between motor input and motor speed to be observed.

The resulting data can be used to estimate a motor model of the form:

PWM ≈ Kv × RPM + Bias

These parameters are subsequently used for feedforward control.

Purpose

Motor characterization helps determine:

Minimum PWM required to overcome static friction.

Approximate PWM-to-RPM relationship.

Differences in motor response.

Initial feedforward parameters for closed-loop control.

2. Proportional Speed Control

File: motor_P/motor_P.ino

The first closed-loop controller implements a P (Proportional) controller for motor speed.

The encoder counts pulses over a fixed sampling interval and converts them into RPM:

RPM = (Encoder Pulses / PPR) × (60 / Sampling Time)

The speed error is calculated as:

Error = Desired RPM − Actual RPM

The controller then calculates:

PWM = Kp × Error

The output is constrained to the valid PWM range.

Objective

The P controller was used as the initial feedback-control stage to understand:

Encoder-based speed measurement.

Feedback error.

Proportional response.

PWM adjustment based on speed error.

Controller gain tuning.

3. PI Speed Control

File: motor_PI/motor_PI.ino

The next stage introduces an Integral term to reduce steady-state error.

The controller uses:

Error = Desired RPM − Actual RPM

and integrates the error over time:

Integral = Integral + Error × dt

The PI controller becomes:

Output = Kp × Error + Ki × Integral

An integral limit is applied to reduce integral windup, where the accumulated error can otherwise cause excessive controller output.

Objective

The PI controller was developed to improve the ability of the motor to reach and maintain the commanded speed despite:

Motor friction

Load variation

Back EMF

Differences between commanded and actual speed

4. Position Control

File: position_PID/position_PID.ino

A separate position-control experiment was implemented using encoder counts as the feedback variable.

The target position is expressed in encoder counts:

Desired Position = Target Revolutions × PPR

The position error is:

Error = Desired Position − Encoder Count

A proportional position controller determines the motor command:

PWM = Kp × Position Error

The motor is stopped when the position error falls within a small tolerance.

Objective

This experiment explored the transition from:

Speed Control
      ↓
Position Feedback
      ↓
Position Regulation

It provided the basis for understanding how encoder feedback can be used not only to control velocity but also to regulate angular position.

5. Four-Motor PID + Feedforward Control

File: completed4WheelPID_ps5Integral...

The project was then expanded from a single motor to four independently controlled motors.

The controller combines:

PID feedback

Feedforward motor model

RPM filtering

Velocity ramping

Wheel synchronization

PID Controller

The controller calculates:

Error = Target RPM − Measured RPM

and:

PID =
Kp × Error
+ Ki × Integral
+ Kd × Derivative

The final command combines the feedback controller with the motor feedforward model:

PWM = Feedforward + PID

where:

Feedforward = Kv × Target RPM + Bias

This reduces the amount of corrective effort required from the feedback controller.

6. RPM Filtering

Encoder measurements can fluctuate because of:

Quantization of encoder pulses

Sampling effects

Motor speed variation

Electrical/mechanical noise

To obtain a more stable speed estimate, an exponential low-pass filter is used:

Filtered RPM =
α × Current RPM
+ (1 − α) × Previous Filtered RPM

with:

α = 0.2

This produces a smoother feedback signal for the PID controller.

7. Velocity Ramping

Sudden changes in commanded motor speed can produce abrupt acceleration and large transient errors.

A velocity ramp was therefore implemented:

Target RPM += Ramp Rate × (Desired RPM − Target RPM)

The desired wheel RPM is first limited to a maximum value:

MAX_RPM = 200

The ramp reduces abrupt changes in motor command and helps produce smoother acceleration and deceleration.

8. Wheel Synchronization

Even when all four motors receive the same nominal command, their actual speeds can differ because of:

Motor manufacturing variation

Wheel friction

Mechanical loading

Battery voltage variation

Differences in gearbox characteristics

To compensate for this, the controller calculates the average wheel RPM:

Average RPM =
(RPM1 + RPM2 + RPM3 + RPM4) / 4

Each wheel then receives a synchronization correction based on:

Synchronization Error =
Average RPM − Individual Wheel RPM

This helps reduce speed mismatch between the four wheels.

9. Mecanum Wheel Inverse Kinematics

Files: practical4WheelPid, completed4WheelPID_ps5Integral...

The robot accepts three high-level motion commands:

Vx     → Forward / backward velocity
Vy     → Left / right velocity
ω      → Rotational velocity

These robot-level commands are converted into four individual wheel angular velocities.

The implemented equations follow the mecanum-drive relationship:

ω1 = (Vx − Vy − Rω) / r

ω2 = (Vx + Vy + Rω) / r

ω3 = (Vx + Vy − Rω) / r

ω4 = (Vx − Vy + Rω) / r

where:

r = Wheel radius
R = Robot radius

The wheel angular velocities are then converted from rad/s to RPM.

This creates the control chain:

Robot Velocity Command
        ↓
Inverse Kinematics
        ↓
Individual Wheel RPM Targets
        ↓
Velocity Ramp
        ↓
PID + Feedforward
        ↓
PWM
        ↓
Motor Driver
        ↓
DC Motors
        ↓
Encoders
        ↓
Measured Wheel RPM
        ↓
Feedback

10. PS5 Controller Integration

File: completed4WheelPID_ps5Integral...

The final stage integrates a PS5 controller using the ESP32.

The analog sticks provide:

Left Stick Y → Vx
Left Stick X → Vy
Right Stick X → Angular velocity (ω)

The joystick values are normalized and scaled according to configurable maximum velocities:

Maximum linear velocity  = 1.0 m/s
Maximum angular velocity = 3.0 rad/s

The resulting velocity command is passed to the mecanum inverse-kinematics module.

This enables intuitive omnidirectional control:

Forward / backward

Left / right strafing

Rotation

Combined translation and rotation

Software Architecture

The final control architecture can be summarized as:

                 PS5 Controller
                       │
                       ▼
              Joystick Processing
                       │
                       ▼
             Robot Velocity (Vx,Vy,ω)
                       │
                       ▼
              Mecanum Inverse Kinematics
                       │
                       ▼
             Desired Wheel RPM × 4
                       │
                       ▼
                Velocity Ramp
                       │
                       ▼
              PID + Feedforward
                       │
                       ▼
                 PWM Commands
                       │
                       ▼
                  Motor Driver
                       │
                       ▼
                 4 DC Motors
                       │
                       ▼
                Wheel Encoders
                       │
                       ▼
                 RPM Estimation
                       │
                       ▼
                  RPM Filter
                       │
                       └───────────────┐
                                       │
                                       ▼
                         Closed-loop correction

Repository Structure

Velocity PID/
│
├── motor_P/
│   └── motor_P.ino
│
├── motor_PI/
│   └── motor_PI.ino
│
├── position_PID/
│   └── position_PID.ino
│
├── motorCharacterization
├── IK_4WheelPid
├── practical4WheelPid
└── completed4WheelPID_ps5Integral...

The files represent different stages and experiments during the development of the robot's control system.

File / Folder

Purpose

motor_P

P speed controller experiment

motor_PI

PI speed controller experiment

position_PID

Encoder-based position control experiment

motorCharacterization

PWM vs RPM characterization

IK_4WheelPid

Four-wheel PID and mecanum kinematics development

practical4WheelPid

Practical four-wheel closed-loop control

completed4WheelPID...

Integrated four-wheel PID, feedforward, synchronization and PS5 control
