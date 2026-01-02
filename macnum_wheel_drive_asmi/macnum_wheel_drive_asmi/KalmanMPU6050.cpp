#include "KalmanMPU6050.h"
#include <math.h>

KalmanMPU6050::KalmanMPU6050() {}

bool KalmanMPU6050::begin() {
    Wire.begin();
    if (!mpu.begin()) return false;

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);

    prevTime = millis();
    delay(1000);
    return true;
}

void KalmanMPU6050::setKalmanParams(float qA, float qB, float rM) {
    q_angle = qA;
    q_bias = qB;
    r_measure = rM;
}

float KalmanMPU6050::kalmanFilter(float newAngle, float newRate, float dt, float &angle, float &bias, float P[2][2]) {
    float rate = newRate - bias;
    angle += dt * rate;

    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += q_bias * dt;

    float S = P[0][0] + r_measure;
    float K0 = P[0][0] / S;
    float K1 = P[1][0] / S;

    float y = newAngle - angle;
    angle += K0 * y;
    bias  += K1 * y;

    float P00_temp = P[0][0];
    float P01_temp = P[0][1];

    P[0][0] -= K0 * P00_temp;
    P[0][1] -= K0 * P01_temp;
    P[1][0] -= K1 * P00_temp;
    P[1][1] -= K1 * P01_temp;

    return angle;
}

void KalmanMPU6050::update() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    unsigned long now = millis();
    float dt = (now - prevTime) / 1000.0;
    prevTime = now;

    // Accelerometer angles
    float accPitch = atan2(
        a.acceleration.y,
        sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)
    ) * 180.0 / PI;

    float accRoll = atan2(-a.acceleration.x, a.acceleration.z) * 180.0 / PI;

    // Kalman filtered
    pitch = kalmanFilter(accPitch, g.gyro.x * 180 / PI, dt, pitch, pitchBias, P_pitch);
    roll  = kalmanFilter(accRoll, g.gyro.y * 180 / PI, dt, roll, rollBias, P_roll);

    // Yaw integration
    yaw += g.gyro.z * 180 / PI * dt;

    // Zero calibration at first run
    if (!calibrated) {
        pitchOffset = pitch;
        rollOffset = roll;
        yawOffset = yaw;
        calibrated = true;
    }

    // Apply offsets
    pitchZeroed = pitch - pitchOffset;
    rollZeroed = roll - rollOffset;
    yawZeroed = yaw - yawOffset;
}