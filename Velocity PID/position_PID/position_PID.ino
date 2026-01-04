// ----------- P CONTROLLER FOR POSITION -----------
const int motorPin = 2;
const int encoderPin = 18;

volatile long encoderCount = 0;
float Kp = 0.49;
float desiredPosition = 5*870;  // target in encoder counts (e.g., 10 rev * 20 pulses/rev)
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
  float error = desiredPosition - encoderCount;

  pwmOutput = Kp * error;
  pwmOutput = constrain(pwmOutput, -255, 255);

  // If position reached, stop
  if (abs(error) < 2) pwmOutput = 0;

  // Apply PWM (handle direction)
  if (pwmOutput >= 0)
    analogWrite(motorPin, pwmOutput);
  else
    analogWrite(motorPin, 0);

  Serial.print("Target: "); Serial.print(desiredPosition);
  Serial.print(" | Actual: "); Serial.print(encoderCount);
  Serial.print(" | Error: "); Serial.println(error);

  delay(50);
}
