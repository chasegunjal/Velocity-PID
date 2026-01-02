void run_motor(char condn, int z) {
  switch (condn) {
    case 'B': //backward
      digitalWrite(dirFL, HIGH);
      analogWrite(pwmFL, z);
      digitalWrite(dirFR, LOW);
      analogWrite(pwmFR, z);
      digitalWrite(dirBL, HIGH);
      analogWrite(pwmBL, z);
      digitalWrite(dirBR, LOW);
      analogWrite(pwmBR, z);
      break;

    case 'F': //forward
      digitalWrite(dirFL, LOW);
      analogWrite(pwmFL, z);
      digitalWrite(dirFR, HIGH);
      analogWrite(pwmFR, z);
      digitalWrite(dirBL, LOW);
      analogWrite(pwmBL, z);
      digitalWrite(dirBR, HIGH);
      analogWrite(pwmBR, z);
      break;

    case 'L':  // prev rotate anticlockwise (CCW)
      digitalWrite(dirFL, LOW);
      analogWrite(pwmFL, z);
      digitalWrite(dirFR, HIGH);
      analogWrite(pwmFR, z);
      digitalWrite(dirBL, HIGH);
      analogWrite(pwmBL, z);
      digitalWrite(dirBR, LOW);
      analogWrite(pwmBR, z);
      break;

    case 'R':   // prev rotate clockwise (CW)
      digitalWrite(dirFL, HIGH);
      analogWrite(pwmFL, z);
      digitalWrite(dirFR, LOW);
      analogWrite(pwmFR, z);
      digitalWrite(dirBL, LOW);
      analogWrite(pwmBL, z);
      digitalWrite(dirBR, HIGH);
      analogWrite(pwmBR, z);
      break;

    case 'S':  // stop
      analogWrite(pwmFL, 0);
      analogWrite(pwmFR, 0);
      analogWrite(pwmBL, 0);
      analogWrite(pwmBR, 0);
      break;

    case 'C':   // rotate clockwise (CW)
      digitalWrite(dirFL, LOW);
      analogWrite(pwmFL, z);
      digitalWrite(dirFR, LOW);
      analogWrite(pwmFR, z);
      digitalWrite(dirBL, LOW);
      analogWrite(pwmBL, z);
      digitalWrite(dirBR, LOW);
      analogWrite(pwmBR, z);
      break;

    case 'A':  // rotate anticlockwise (CCW)
      digitalWrite(dirFL, HIGH);
      analogWrite(pwmFL, z);
      digitalWrite(dirFR, HIGH);
      analogWrite(pwmFR, z);
      digitalWrite(dirBL, HIGH);
      analogWrite(pwmBL, z);
      digitalWrite(dirBR, HIGH);
      analogWrite(pwmBR, z);
      break;

    // case 'X':  // diagonal front-right
    //   digitalWrite(dirFL, LOW);
    //   analogWrite(pwmFL, z);
    //   digitalWrite(dirFR, LOW);
    //   analogWrite(pwmFR, 0);
    //   digitalWrite(dirBL, LOW);
    //   analogWrite(pwmBL, 0);
    //   digitalWrite(dirBR, LOW);
    //   analogWrite(pwmBR, z);
    //   break;

    // case 'X':  // diagonal front-left
    //   digitalWrite(dirFL, LOW);
    //   analogWrite(pwmFL, 0);
    //   digitalWrite(dirFR, LOW);
    //   analogWrite(pwmFR, z);
    //   digitalWrite(dirBL, LOW);
    //   analogWrite(pwmBL, z);
    //   digitalWrite(dirBR, LOW);
    //   analogWrite(pwmBR, 0);
    //   break;

    // case 'X':  // diagonal back-right
    //   digitalWrite(dirFL, HIGH);
    //   analogWrite(pwmFL, 0);
    //   digitalWrite(dirFR, HIGH);
    //   analogWrite(pwmFR, z);
    //   digitalWrite(dirBL, HIGH);
    //   analogWrite(pwmBL, z);
    //   digitalWrite(dirBR, HIGH);
    //   analogWrite(pwmBR, 0);
    //   break;

    // case 'X':  // diagonal back-left
    //   digitalWrite(dirFL, HIGH);
    //   analogWrite(pwmFL, z);
    //   digitalWrite(dirFR, HIGH);
    //   analogWrite(pwmFR, 0);
    //   digitalWrite(dirBL, HIGH);
    //   analogWrite(pwmBL, 0);
    //   digitalWrite(dirBR, HIGH);
    //   analogWrite(pwmBR, z);
    //   break;
  }
}
