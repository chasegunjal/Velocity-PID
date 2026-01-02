#include <ps5.h>
#include <ps5Controller.h>
#include <ps5_int.h>
#include <esp_system.h>



// #ifndef KALMAN_MPU6050_H
// #define KALMAN_MPU6050_H

// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

#include "KalmanMPU6050.h"

KalmanMPU6050 mpuSensor;
//#include <math.h>


const int pwmBL = 19, pwmFR = 23, pwmFL = 27, pwmBR = 18; //pwmfr = 23
const int dirBL = 16, dirFR = 4, dirFL = 13, dirBR = 17; 

// constants for drive:
float Lx, Rx, Ly, Ry, z;  //used datatype cording to ps5 library , if error occured stick to float
float control;

//relay pin defination:
const int RELAY_1_PIN = 32;  // back2
const int RELAY_2_PIN = 33; // back1
const int RELAY_3_PIN = 25; // front

// ultrasonic
const int trigPin = 14;
const int echoPin = 12;

float duration, distance;

unsigned long ultraTimer = 0;
const unsigned long ultraInterval = 100; // ms





void setup() {

  Serial.begin(115200);
 ps5.begin("88:03:4C:93:FE:13");
//  ps5.begin("BC:C7:46:0A:66:DA");

 mpuSensor.begin();




  // drive pinmodes :

  pinMode(pwmFL, OUTPUT);
  pinMode(pwmFR, OUTPUT);
  pinMode(pwmBL, OUTPUT);
  pinMode(pwmBR, OUTPUT);
  pinMode(dirFL, OUTPUT);
  pinMode(dirFR, OUTPUT);
  pinMode(dirBL, OUTPUT);
  pinMode(dirBR, OUTPUT);

   //relay pins pinmode:

  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);

  //ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);


  //ps5 drive controls:

  Lx = ps5.LStickX();
  Ly = ps5.LStickY();
  Ry = ps5.RStickY();
  Rx = ps5.RStickX();

  

  if (ps5.isConnected()) {
    Serial.println("Connected!");
  }

  while (!ps5.isConnected()) {
    Serial.println("Connecting to PS5 Controller...");
    delay(500);
  }
  Serial.println("Connected to PS5 Controller!");

  //digitalWrite(RELAY_3_PIN, LOW);


}

float maxPitch = 0;

void loop() {
  Lx = ps5.LStickX();
  Ly = ps5.LStickY();
  Ry = ps5.RStickY();
  Rx = ps5.RStickX();

  if (Lx || Ly || Ry || Rx) {  //for 3 wheel drive
    driveconditions();
  }

  if (ps5.Up() || ps5.Down() || ps5.Left() || ps5.Right()) {  //for relay
    relay();
  }
   if ( ps5.Circle()){
    Serial.println("Automation Started");
    ultra();
    automate();
    Serial.println("Automation End");

   }

  if (ps5.Square()){
    run_motor('S', 0);

  }

    mpuSensor.update();
    float temp = mpuSensor.getPitch();
    if(temp < maxPitch){
      maxPitch = mpuSensor.getPitch();
    }
    Serial.print("Pitch: "); 
    Serial.print(mpuSensor.getPitch(), 2);
    
    Serial.print(" | Max: ");
    Serial.println(maxPitch, 2);

    // Serial.print(" | Roll: "); Serial.print(mpuSensor.getRoll(), 2);
    // Serial.print(" | Yaw: "); Serial.println(mpuSensor.getYaw(), 2);

    delay(5);  
  //automate();

}