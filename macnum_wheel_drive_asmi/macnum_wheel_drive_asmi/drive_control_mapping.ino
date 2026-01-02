
float mapping(float control) {
  float z;
  if ((-128 <= control) && (control <= -30)) {
    z = map(control, -30, -128, 0, 150);           //mapping negative values
  } else if ((30 <= control) && (control <= 128))  //mapping positive values
  {
    z = map(control, 30, 128, 0, 150);
  } else
    z = 0;
  return z;
}