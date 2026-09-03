Overview

A 4-wheel mecanum robot control system developed using an ESP32, DC geared motors, encoders, and a PS5 controller.

The project focuses on developing a closed-loop motor control system that converts high-level robot motion commands into individual wheel speeds and regulates them using encoder feedback.

The controller was developed progressively from single-motor experiments to a complete four-wheel motion-control system.

Hardware
      ESP32 — Main controller
      4 × DC geared motors
      4 × Quadrature encoders
      Motor driver
      4 × Mecanum wheels
      PS5 controller
      Battery / power supply

| Parameter           |   Value |
| ------------------- | ------: |
| Wheel radius        |  0.05 m |
| Robot radius        |  0.18 m |
| Encoder resolution  | 850 PPR |
| Maximum wheel speed | 200 RPM |

Objective:-
      Develop encoder-based closed-loop motor speed control.
      Experiment with P, PI and PID controllers.
      Characterize motor response for feedforward control.
      Synchronize the speed of all four wheels.
      Implement mecanum wheel inverse kinematics.
      Enable wireless omnidirectional control using a PS5 controller.


Control Architecture:-

                 PS5 Controller
                       │
                       ▼
              Robot Velocity (Vx, Vy, ω)
                       │
                       ▼
            Mecanum Inverse Kinematics
                       │
                       ▼
              Individual Wheel RPM
                       │
                       ▼
                Velocity Ramping
                       │
                       ▼
              PID + Feedforward
                       │
                       ▼
                     PWM
                       │
                       ▼
                 Motor Driver
                       │
                       ▼
                  DC Motors
                       │
                       ▼
                   Encoders
                       │
                       ▼
                RPM Feedback
                       └──────────► Controller

Key Features:-

Encoder-based closed-loop speed control
P, PI and PID controller experiments
Motor characterization and feedforward modeling
RPM filtering for smoother feedback
Velocity ramping for smoother acceleration
Four-wheel speed synchronization
Mecanum inverse kinematics for omnidirectional movement
PS5 wireless controller integration
Independent control of all four drive motors
Development

The controller was developed incrementally:

Motor Characterization
        ↓
P Speed Control
        ↓
PI Speed Control
        ↓
Position Control
        ↓
4-Motor PID
        ↓
PID + Feedforward
        ↓
Wheel Synchronization
        ↓
Mecanum Inverse Kinematics
        ↓
PS5 Controller Integration
Results

The project resulted in a four-wheel closed-loop motion-control framework capable of:

Measuring individual wheel RPM using encoders.
Regulating wheel speed using PID feedback.
Compensating motor behavior using feedforward control.
Reducing wheel-speed mismatch through synchronization.
Converting robot velocity commands into individual wheel RPM targets.
Controlling the robot wirelessly using a PS5 controller.
Technologies

ESP32 · Embedded C/C++ · DC Motor Control · Encoders · PWM · PID Control · Feedforward Control · Mecanum Kinematics · Robotics · PS5 Controller

Repository Structure
Velocity PID/
│
├── motor_P/
├── motor_PI/
├── position_PID/
├── motorCharacterization
├── IK_4WheelPid
├── practical4WheelPid
└── completed4WheelPID_ps5Integral...

Each file/folder represents a different stage of the controller development, from basic motor experiments to the integrated four-wheel system.

Future Improvements:-

      Full quadrature direction decoding
      Bidirectional motor control
      Individual PID tuning for each motor
      Wheel odometry
      IMU-based heading correction
      Closed-loop robot position control
