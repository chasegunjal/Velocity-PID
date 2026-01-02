void driveconditions() {
  // Threshold to ignore joystick noise
  int threshold = 30;

  if (Ly > threshold) {  // forward
  
    //Serial.println("forward");
    z = mapping(Ly);
    run_motor('F', z);
  } 
  else if (Ly < -threshold) {  // backward
    //Serial.println("backward");
    z = mapping(Ly);
    run_motor('B', abs(z));
  } 
  else if (Lx < -threshold) {  // left
    //Serial.println("left");
    z = mapping(Lx);
    run_motor('L', abs(z));
  } 
  else if (Lx > threshold) {  // right
    //Serial.println("right");
    z = mapping(Lx);
    run_motor('R', abs(z));
  } 
  else if (Rx > threshold) {  // rotate clockwise
    //Serial.println("clockwise");
    z = mapping(Rx);
    run_motor('C', abs(z));
  } 
  else if (Rx < -threshold) {  // rotate anticlockwise
   // Serial.println("anticlockwise");
    z = mapping(Rx);
    run_motor('A', abs(z));
  } 
  else if (z > 0) {  // gradual stop
    //Serial.println("stop");
    z = -10;
    run_motor('S', z);
    delay(100);
  } 
  else {  // full stop
  
   // Serial.println("full stop");
    z = 0;
    run_motor('S', z);
  }
}
