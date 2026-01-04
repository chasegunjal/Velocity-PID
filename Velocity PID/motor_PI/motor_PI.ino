// ----------- P CONTROLLER FOR SPEED -----------
const int motorPin = 2 ;  // PWM pin connected to motor driver input
const int encoderPin = 18;  // Encoder signal pin

volatile float encoderCount = 0.0;
unsigned long lastTime = 0;
float actualRPM = 0;
float desiredRPM = 100;  // Target speed
float Kp = 1.439;          // Proportional gain
int pwmOutput = 0;

void encoderISR() {
  encoderCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(motorPin, OUTPUT);
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPin), encoderISR, RISING);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 100) { // Every 100ms
    // ---- Calculate RPM ----
    actualRPM = (encoderCount * ) / 20.0; // example: 20 pulses per rev
    encoderCount = 0;

    // ---- Error ----
    error = desiredRPM - actualRPM;

    // ---- Integral term ----
    integral += error * 0.1;   // 0.1 = time step in seconds (100ms)
    integral = constrain(integral, -500, 500);  // prevent runaway integral (windup)

    // ---- PI Output ----
    float output = (Kp * error) + (Ki * integral);
    pwmOutput = constrain(output, 0, 255);

    // ---- Apply PWM ----
    analogWrite(motorPin, pwmOutput);

    // ---- Debug ----
    Serial.print("Setpoint: "); Serial.print(desiredRPM);
    Serial.print(" | Actual: "); Serial.print(actualRPM);
    Serial.print(" | PWM: "); Serial.print(pwmOutput);
    Serial.print(" | Error: "); Serial.println(error);

    lastTime = currentTime;
  }
}
