#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 3
#define BRIGHTNESS 100
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

Servo tiltServo;  // create servo object to control a servo
Servo panServo;

int i = 0;
int recoil = 5;
byte x_pos[] = {
   45,  41,  36,  31,  27,  19,  15,  10,   5,   2,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   53,   0,   0,   0,   0,   0,   0,   0,   0,   2  
};

byte y_pos[] = {
   72,  72,  71,  71,  71,  71,  72,  73,  74,  74,
    0,   0,   0,   0,   0,   0,   0,   0,   0,  80,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  117,   0,   0,   0,   0,   0,   0,   0,   0, 120  
};

byte hits[] = {
    1,   1,   1,   1,   1,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
    0,   0,   0,   0,   0,   1,   0,   0,   0,   1,
    0,   1,   1,   0,   0,   1,   0,   0,   0,   1,
    0,   0,   0,   0,   0,   1,   0,   0,   0,   1,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    1,   1,   1,   1,   0,   0,   0,   0,   0,   0  
};

void setup() {
  pinMode(13, OUTPUT); // for LED status
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  tiltServo.attach(10);  // attaches the servo on pin 9 to the servo object
  panServo.attach(9);
  Serial.begin(9600);
  tiltServo.write(95);    
  panServo.write(20);
  delay(1000);
}

void loop() {

    if (Serial.available() > 0)
  {
    char cmd = Serial.read();

    switch (cmd) {
      case 'c':
        i = Serial.parseInt();
        tiltServo.write(y_pos[i]);    // tell tilt servo to go to position A0 on the board
        panServo.write(x_pos[i]);
        Serial.println("Y: " + String(y_pos[i]) + " X: " + String(x_pos[i]));
        delay(500);
        tiltServo.write(y_pos[i] - recoil);  
        delay(500);
        tiltServo.write(y_pos[i]);
          if(hits[i]){
            digitalWrite(13,HIGH);
            strip.setPixelColor(i, strip.Color(255, 0, 0));
            strip.setPixelColor(i+1, strip.Color(255, 0, 0));
            strip.setPixelColor(i+2, strip.Color(255, 0, 0));
            strip.show();
          }
        delay(1000);
        tiltServo.write(95);    
        panServo.write(20);
        delay(1000);
        Serial.println("RESET");
        digitalWrite(13,LOW);

        
        Serial.read(); // Clear the buffer
        break;

      default:
        break;
    }
  }
    
  /*  for(int i = 0; i < 12; i++){
      tiltServo.write(y_pos[i]);    // tell tilt servo to go to position A0 on the board
      panServo.write(x_pos[i]);
      Serial.println("Y: " + String(y_pos[i]) + " X: " + String(x_pos[i]));
        if(hits[i]){
          digitalWrite(13,HIGH);
        }
      delay(3000);
      tiltServo.write(95);    
      panServo.write(20);
      delay(1000);
      Serial.println("RESET");
      digitalWrite(13,LOW);
    }*/
   
    
}
