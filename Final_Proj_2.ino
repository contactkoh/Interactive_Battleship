/*
 * MP3 
 */

#include <SoftwareSerial.h>
#define ARDUINO_RX 5//should connect to TX of the Serial MP3 Player module  (b/c pin 5 = Arduino's RX)
#define ARDUINO_TX 6//connect to RX of the module  (b/c pin 6 = Arduino's TX)
SoftwareSerial mySerial(ARDUINO_RX, ARDUINO_TX);
static int8_t Send_buf[8] = {0} ;

#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_PLAY_W_VOL 0X22 //22
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_PREVIOUS 0X02
#define CMD_NEXT 0X01
#define CMD_RAND 0X03




/*
 * Bluetooth
 */

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

  /*
   * Mp3
   */
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(500);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  sendCommand(CMD_PLAY_W_VOL, 0X1E01);//play the first song with volume 30 class
  Serial.println("Setting Up initially...");

  
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
        
        /*
         * MP3
         */
        sendCommand(CMD_PLAY_W_VOL, 0X1E03);//play the first song with volume 30 class
        delay(2000);
        // turn LED on:
        //Serial.println("If Button STate HIGH" + buttonState);
        //digitalWrite(ledPin, HIGH);
        sendCommand(CMD_PLAY_W_VOL, 0X1E02);//play the first song with volume 30 class
        delay(2000);
        sendCommand(CMD_PLAY_W_VOL, 0X1E01);//play the first song with volume 30 class
        
        
        //tiltServo.write(y_pos[i] - recoil);  
        //delay(500);
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


void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Serial.println("Inside sendCommand");

  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for(uint8_t i=0; i<8; i++)//
  {
    mySerial.write(Send_buf[i]) ;
  }
}
