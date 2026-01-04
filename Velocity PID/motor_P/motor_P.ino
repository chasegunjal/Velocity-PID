// ----------- P CONTROLLER FOR SPEED -----------
const int motorPin = 2 ;  // PWM pin connected to motor driver input
const int motorDir = 3 ;  // PWM pin connected to motor driver input
const int encoderAPin = 18;  // Encoder signal pin
const int encoderBPin = 19;  // Encoder signal pin

volatile long encoderCount = 0;
unsigned long lastTime = 0;
float actualRPM = 0;
float desiredRPM = 100;  // Target speed
float Kp = 1.5;          // Proportional gain
int pwmOutput = 0;

void encoderISR() {
  encoderCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(motorPin, OUTPUT);
  pinMode(encoderAPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderAPin), encoderISR, RISING);
}

void loop() {
  //analogWrite(motorPin, 255);
  //digitalWrite(motorPin, 30);
  //Serial.println(encoderCount/793.0);
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 1000) { // Every 100ms = 0.1s
    // ---- Calculate RPM ----
    noInterrupts();
    long count = encoderCount;
    encoderCount = 0;
    interrupts();
    actualRPM = ( (float) count/870.0)*60; // Example: 20 pulses per rev
    //Serial.println(actualRPM);
    //Serial.println(count);

        // ---- Error ----
    float error = desiredRPM - actualRPM;

   //---- P Control ----

    pwmOutput = Kp * error;

    // ---- Limit PWM ----
    pwmOutput = constrain(pwmOutput, 0, 255);

    // ---- Apply PWM ----
    analogWrite(motorPin, pwmOutput);

//    ---- Debug ----
    Serial.print(" | Desired: "); Serial.print(desiredRPM);
    Serial.print(" | Actual: "); 
    Serial.print(actualRPM);
    Serial.print(" | PWM: "); Serial.println(pwmOutput);
  lastTime = currentTime;
  }
  // if(currentTime - lastTime >= 100){
  //       // ---- Error ----
  //   float error = desiredRPM - actualRPM;

  //   //---- P Control ----
  //   pwmOutput = Kp * error;

  //   // ---- Limit PWM ----
  //   pwmOutput = constrain(pwmOutput, 0, 255);

  //   // ---- Apply PWM ----
  //   analogWrite(motorPin, pwmOutput);

  //   // ---- Debug ----
  //   Serial.print("Desired: "); Serial.print(desiredRPM);
  //   Serial.print(" | Actual: "); Serial.print(actualRPM);
  //   Serial.print(" | PWM: "); Serial.println(pwmOutput);
  //}
}
