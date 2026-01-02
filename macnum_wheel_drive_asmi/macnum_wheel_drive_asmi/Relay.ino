void relay()   //gripper
{
  
    // L1 Button Controls Relay 1 jaw open
    if (ps5.Up()) 
    {
      digitalWrite(RELAY_1_PIN, HIGH);
      digitalWrite(RELAY_2_PIN, LOW);
    } else if (ps5.Down()) 
    {
      digitalWrite(RELAY_2_PIN, HIGH);
      digitalWrite(RELAY_1_PIN, LOW);
    }

    if (ps5.Left()) 
    {
      digitalWrite(RELAY_3_PIN, HIGH);
    } else if(ps5.Right())
    {
      digitalWrite(RELAY_3_PIN, LOW);
    }
  
}
  