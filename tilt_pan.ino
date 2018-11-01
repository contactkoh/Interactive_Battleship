#include <Servo.h>

Servo tiltServo;  // create servo object to control a servo
Servo panServo;
// twelve servo objects can be created on most boards

int tiltRead = 0;
int panRead = 0;

void setup() {
  tiltServo.attach(9);  // attaches the servo on pin 9 to the servo object
  panServo.attach(8);
  Serial.begin(9600);
}

void loop() {
  if(analogRead(A0)!=tiltRead){
    tiltServo.write(map(analogRead(A0),0,1000,0,180));    // tell tilt servo to go to position being read in on A0
    tiltRead = analogRead(A0);
  }
  if(analogRead(A1)!=panRead){
    panServo.write(map(analogRead(A1),0,1000,0,180));    // tell pan servo to go to position being read in on A1
    panRead = analogRead(A1);
  }

  delay(200);                       // waits 20ms for the servo to reach the position
  
}
