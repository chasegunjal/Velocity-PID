void automate(){
  while ( distance > 10  ){
    Serial.print("Distance == ");
    Serial.println(distance);
    ultra();
    run_motor('F' , 100);
    Serial.println(distance);

    if(distance > 400){  
      Serial.println("Automation Exit");
      run_motor('S', 0);
      return;
    }
  }

  run_motor('S', 0);
   digitalWrite(RELAY_3_PIN, LOW);
   mpuDelay('P', -23.4, 0.5, 15);
  Serial.println("Front Piston Actuate");
   run_motor('F' , 100);
   mpuDelay('P', -26.7, 1, 15);
   digitalWrite(RELAY_3_PIN, HIGH);
   digitalWrite(RELAY_2_PIN, LOW);
   digitalWrite(RELAY_1_PIN, HIGH);
   
  Serial.println("Front Piston Dectuate");
  Serial.println("Back Piston Actuate");
   mpuDelay('P', 0.1, 1, 10);
   run_motor('S', 0);


   digitalWrite(RELAY_2_PIN, HIGH);
   digitalWrite(RELAY_1_PIN, LOW);

   
  Serial.println("Back Piston Dectuate");
   delay(1000);
   run_motor('F' , 150);
   delay(500);
   run_motor('S', 0);

   
  Serial.println("Done");
  
}