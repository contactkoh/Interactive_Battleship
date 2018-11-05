// scanning radar on, led on

//byte value;

void state1Setup() {

  Serial.println("Current State is :");
  Serial.println(state);
  
  myservoLED.attach(7); // attach radar LED scan servo on d7

}

void state1Loop() {
  
/*
 * servo LED Sweep
 */
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservoLED.write(pos);              // tell servo to go to position in variable 'pos'
    delay(speedLED);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservoLED.write(pos);              // tell servo to go to position in variable 'pos'
    delay(speedLED);                       // waits 15ms for the servo to reach the position
  }
  
  Serial.println(myservoLED.read());
  Serial.println("from ServoLED.");

  
}

void doState1() {
  if (lastState != 1) { state1Setup(); }
  state1Loop();
}
