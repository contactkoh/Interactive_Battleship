// stop scanning, enter manual target mode

void state0Setup() {
  myservoLR.attach(9);  // attaches the servo on digital pin 9 to the servo object
  myservoUD.attach(10);
  
  Serial.println("Current State is :");
  Serial.println(state);
  
}

void state0Loop() {
  valLR = analogRead(potpinLR);            // reads the value of the potentiometer (value between 0 and 1023)
  valUD = analogRead(potpinUD);
  valLR = map(valLR, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  valUD = map(valUD, 0, 1023, 0, 180);
  myservoLR.write(valLR);                  // sets the servo position according to the scaled value
  myservoUD.write(valUD);
  delay(15);                           // waits for the servo to get there

  Serial.println(myservoLR.read());  
  Serial.println("from Servo LR.");
  Serial.println(myservoUD.read());
  Serial.println("from Servo UD.");


}

void doState0() {
  if (lastState != 0) { //state0Setup();
    }
  state0Loop();
}
