void mpuDelay(char orien, float target, float tol, int stableCountNeeded) {
  //const float tol = 0.5;                 // tolerance band
  // const int stableCountNeeded = 15;      // ~30 samples stability
  int stableCount = 0;

  float angle = 0;

  while (1) {
    mpuSensor.update();

    switch (orien) {
      case 'P': angle = mpuSensor.getPitch(); break;
      case 'R': angle = mpuSensor.getRoll();  break;
      case 'Y': angle = mpuSensor.getYaw();   break;
    }

    if (abs(angle - target) <= tol) {
      stableCount++;
    } else {
      stableCount = 0;   // instability detected → reset
    }

    if (stableCount >= stableCountNeeded) {
      break;             // 🎉 angle is stable
    }

    delay(10); // sampling period (10 ms → 300 ms total)
  }
}
