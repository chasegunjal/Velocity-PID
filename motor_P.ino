// ----------- P CONTROLLER FOR SPEED -----------
#include <Arduino.h>
#include <ISR>
#include <RPM_function>
#include <PID_function>
#include "globals"
// #include "headers"

const int motorPin = 2;     // PWM pin connected to motor driver input
const int motorDir = 3;     // PWM pin connected to motor driver input
const int encoderAPin = 18; // Encoder signal pin
const int encoderBPin = 19; // Encoder signal pin

float rpmFiltered = 0;

volatile unsigned long lastPulse = 0;
volatile unsigned long pulsePeriod = 0;

const int ppr = 870;
float actualrpm = 0;
float desiredRPM = 100; // Target speed
float Kp = 2;           // Proportional gain
float integral = 0;
float Ki = 0;
float Kd = 0;

int pwmOutput = 0;
int pwm_bias = 0; // assign the pwm w.r.t target rpm

void isr();
void rpm();
void pid();

void setup()
{
  Serial.begin(115200);
  pinMode(motorPin, OUTPUT);
  pinMode(encoderAPin, INPUT_PULLUP);
  pinMode(encoderBPin, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(encoderAPin), pulse_timing_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderAPin), pulse_timing_isr, RISING);
}

void loop()
{
  readPIDFromSerial();
  get_rpm();
}
