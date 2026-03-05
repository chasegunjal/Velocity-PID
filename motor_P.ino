#include <Arduino.h>

// -------- MOTOR CONFIG --------
#define NUM_MOTORS 4

int motorPWM[NUM_MOTORS] = {2, 4, 6, 8};
int motorDIR[NUM_MOTORS] = {3, 5, 7, 9};

int encoderA[NUM_MOTORS] = {18, 19, 20, 21};
int encoderB[NUM_MOTORS] = {22, 23, 24, 25};

// -------- ENCODER VARIABLES --------
volatile unsigned long lastPulse[NUM_MOTORS] = {0};
volatile unsigned long pulsePeriod[NUM_MOTORS] = {0};

const int PPR = 850;

// -------- PID VARIABLES --------
float desiredRPM[NUM_MOTORS] = {80,80,80,80};

float actualRPM[NUM_MOTORS] = {0};
float rpmFiltered[NUM_MOTORS] = {0};

float Kp = 0.5;
float Ki = 0;
float Kd = 0;

float integral[NUM_MOTORS] = {0};

int pwmOutput[NUM_MOTORS] = {0};
int pwmBias[NUM_MOTORS] = {0};

unsigned long prevTime = 0;
float dt = 0;


// -------- INTERRUPTS --------
void IRAM_ATTR encoderISR0()
{
    unsigned long now = micros();
    pulsePeriod[0] = now - lastPulse[0];
    lastPulse[0] = now;
}

void IRAM_ATTR encoderISR1()
{
    unsigned long now = micros();
    pulsePeriod[1] = now - lastPulse[1];
    lastPulse[1] = now;
}

void IRAM_ATTR encoderISR2()
{
    unsigned long now = micros();
    pulsePeriod[2] = now - lastPulse[2];
    lastPulse[2] = now;
}

void IRAM_ATTR encoderISR3()
{
    unsigned long now = micros();
    pulsePeriod[3] = now - lastPulse[3];
    lastPulse[3] = now;
}


// -------- RPM FILTER --------
void rpmFilter(int i)
{
    float alpha = 0.2;

    rpmFiltered[i] =
        alpha * actualRPM[i] +
        (1 - alpha) * rpmFiltered[i];
}


// -------- PID CONTROL --------
void PIDControl(int i)
{
    float error = desiredRPM[i] - rpmFiltered[i];

    if(abs(error) < 1)
        error = 0;

    integral[i] += error * dt;

    integral[i] = constrain(integral[i], -500, 500);

    pwmOutput[i] =
        pwmBias[i] +
        (Kp * error) +
        (Ki * integral[i]);

    pwmOutput[i] = constrain(pwmOutput[i], 0, 255);

    analogWrite(motorPWM[i], pwmOutput[i]);
}


// -------- RPM CALCULATION --------
void getRPM(int i)
{
    if(pulsePeriod[i] == 0) return;

    actualRPM[i] =
        60.0e6 /
        (pulsePeriod[i] * PPR);

    rpmFilter(i);
}


// -------- SETUP --------
void setup()
{
    Serial.begin(115200);

    for(int i=0;i<NUM_MOTORS;i++)
    {
        pinMode(motorPWM[i], OUTPUT);
        pinMode(motorDIR[i], OUTPUT);

        pinMode(encoderA[i], INPUT_PULLUP);
        pinMode(encoderB[i], INPUT_PULLUP);
    }

    attachInterrupt(
        digitalPinToInterrupt(encoderA[0]),
        encoderISR0,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(encoderA[1]),
        encoderISR1,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(encoderA[2]),
        encoderISR2,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(encoderA[3]),
        encoderISR3,
        RISING);

    prevTime = millis();
}


// -------- MAIN LOOP --------
void loop()
{
    unsigned long now = millis();
    dt = (now - prevTime)/1000.0;
    prevTime = now;

    for(int i=0;i<NUM_MOTORS;i++)
    {
        getRPM(i);
        PIDControl(i);
    }

    // -------- DEBUG --------
    Serial.print("RPM: ");
    for(int i=0;i<NUM_MOTORS;i++)
    {
        Serial.print(rpmFiltered[i]);
        Serial.print(" ");
    }

    Serial.print(" | PWM: ");
    for(int i=0;i<NUM_MOTORS;i++)
    {
        Serial.print(pwmOutput[i]);
        Serial.print(" ");
    }

    Serial.println();
}
