/*
 
 */
#include <Wire.h>
#include "Adafruit_VCNL4010.h"

Adafruit_VCNL4010 vcnl;

#include <Servo.h>

Servo myservoLR;  // create servo object to control a servo Left-Right
Servo myservoUD;  // servo Up-Down

Servo myservoLED; 
int speedLED = 15; // LED servo speed

int potpinLR = 0;  // analog pin used to connect the potentiometer
int potpinUD = 1;
int valLR;    // variable to read the value from the analog pin
int valUD;

int pos = 0;    // variable to store the LED servo position


int ambient = 0;
int proximity = 0;
int speedAmbProx = 15; //frequency of measuring for Ambient Proximity Sensor

int Led_Button = LED_BUILTIN; //  or LED_BUILTIN
int Button = 6; // button at pin 6

//Blink withiout Delay function borrowed
// constants won't change. Used here to set a pin number:
const int ledPin =  2; //LED_BUILTIN;// the number of the LED pin
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)



int state = 1;
int lastState = 1;


void setup() {
  Serial.begin(9600);

  //Button Activated
  pinMode(Led_Button, OUTPUT);
  pinMode(Button, INPUT);
  

  Serial.println("VCNL4010 test");
  if (! vcnl.begin()){
    Serial.println("Proximity Sensor not found :(");
    while (1);
  }
  Serial.println("Found VCNL4010");

  pinMode(ledPin, OUTPUT);
  
}

void loop() {
   Serial.println("Inside Main Loop");


     //Button Activated
   if(digitalRead(Button) == HIGH)
   {
    digitalWrite(Led_Button, HIGH);
    Serial.println("Button Pressed");
    //shoot laset
   }else
   {
    digitalWrite(Led_Button,LOW);
    Serial.println("Button Released");
   }
  
   
  
   unsigned long currentMillis = millis();

   if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;

      //also read the amv/ prox  values

      
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
   }
    
   ambient = vcnl.readAmbient();
   proximity = vcnl.readProximity();
   Serial.print("Ambient: "); Serial.println(ambient);
   Serial.print("Proximity: "); Serial.println(proximity);
   delay(speedAmbProx);

   // setting state
   if (ambient < 5 || proximity > 3000) {
    state = 0;  // stop scanning, enter manual target mode
   } else {
    state = 1;  // radar scanning with led
   }
   
  // entering state
  switch (state) {
  case 0:    
    doState0();
    break;
  case 1:    
    doState1();
    break;
  /*
  case 2:    
    doState2();
    break; */
  } 
  lastState = state;
}
