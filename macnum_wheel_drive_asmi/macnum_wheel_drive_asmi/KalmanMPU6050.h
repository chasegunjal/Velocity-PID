#ifndef KALMAN_MPU6050_H
#define KALMAN_MPU6050_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class KalmanMPU6050 {
public:
    KalmanMPU6050();

    // Initialize MPU6050 sensor
    bool begin();

    // Update angles and return zeroed values
    void update();

    // Get filtered angles
    float getPitch() const { return pitchZeroed; }
    float getRoll()  const { return rollZeroed; }
    float getYaw()   const { return yawZeroed; }

    // Optional: set Kalman parameters
    void setKalmanParams(float q_angle, float q_bias, float r_measure);

private:
    Adafruit_MPU6050 mpu;

    // Kalman filter parameters
    float q_angle = 0.01;
    float q_bias  = 0.01;
    float r_measure = 0.01;

    // Pitch & Roll Kalman states
    float pitch = 0, roll = 0;
    float pitchBias = 0, rollBias = 0;

    float P_pitch[2][2] = {{1, 0}, {0, 1}};
    float P_roll[2][2]  = {{1, 0}, {0, 1}};

    // Yaw (gyro integrated)
    float yaw = 0;

    // Zero offsets
    float pitchOffset = 0;
    float rollOffset  = 0;
    float yawOffset   = 0;

    bool calibrated = false;
    unsigned long prevTime = 0;

    float pitchZeroed = 0;
    float rollZeroed = 0;
    float yawZeroed = 0;

    float kalmanFilter(float newAngle, float newRate, float dt, float &angle, float &bias, float P[2][2]);
};

#endif