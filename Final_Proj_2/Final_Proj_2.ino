/*
 * MP3 
 */

#include <SoftwareSerial.h>
#define ARDUINO_RX 5//should connect to TX of the Serial MP3 Player module  (b/c pin 5 = Arduino's RX)
#define ARDUINO_TX 3//connect to RX of the module  (b/c pin 6 = Arduino's TX)  FOR THIS FILE, TRY pin 3 instead of 6
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
 * Led firing arms
 */
 const int ledPin = 12;      // the number of the LED pin. or LED_BUILTIN



/*
 * LED strips 
 */
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 100
#define BRIGHTNESS 100
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);


/*
 * Servo
 */
#include <Servo.h>

Servo tiltServo;  // create servo object to control a servo
Servo panServo;

int i = 0;
//int recoil = 5;
//Mapping Serial input from BT, to Servo motor value
byte x_pos[] = {
   36,  36,  36,  36,  36,  36,  36,  36,  36,  36, 
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 
   28,  28,  28,  28,  28,  28,  28,  28,  28,  28,
   24,  24,  24,  24,  24,  24,  24,  24,  24,  24,
   20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
   16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
   12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0 
};

byte y_pos[] = {
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120,
   93,  96,  99, 102, 105, 108, 111, 114, 117, 120 
};

byte hits[] = {
    1,   1,   1,   1,   1,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
    0,   0,   0,   0,   0,   4,   0,   0,   0,   2,
    0,   5,   5,   0,   0,   4,   0,   0,   0,   2,
    0,   0,   0,   0,   0,   4,   0,   0,   0,   2,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    3,   3,   3,   3,   0,   0,   0,   0,   0,   0  
};

byte shipstatus[] = {
  0, 0, 0, 0, 0, 0
};

byte shiplength[] = {
  0, 5, 4, 4, 3, 2
};

void setup() {

  /*
   * Mp3
   */
  Serial.begin(9600);   // BT serial input
  mySerial.begin(9600);  //mp3 module Serial
  delay(500);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  sendCommand(CMD_PLAY_W_VOL, 0X1E01);//play the first song with volume 30 class
  //Serial.println("Setting Up initially...");
  pinMode(13, OUTPUT); // for LED status
  
  pinMode(ledPin, OUTPUT); // for LED Firing Arms

  /*
   * LED Strip
   */
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();

  
  tiltServo.attach(10);  // attaches the servo on pin 9 to the servo object
  panServo.attach(9);
  tiltServo.write(75);  //initial position  center  . orig 90
  panServo.write(18);
  shipstatus[1] = 0;  // set all ship hit-counts to 0
  shipstatus[2] = 0;
  shipstatus[3] = 0;
  shipstatus[4] = 0;
  shipstatus[5] = 0;
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
  }
  strip.show();
  delay(1000);
}

void loop() {

  if (Serial.available() > 0)  //reading from BT Serial input
  {
    char cmd = Serial.read();

    switch (cmd) {
      case 'c':        
        i = Serial.parseInt();
        tiltServo.write(y_pos[i]);    // tell tilt servo to go to position A0 on the board
        panServo.write(x_pos[i]);
        //Serial.println("Y: " + String(y_pos[i]) + " X: " + String(x_pos[i]));
        delay(500);

        //play Open fire
        int randTrackNo;
        randTrackNo = random(2, 4);
        if (randTrackNo == 3) { sendCommand(CMD_PLAY_W_VOL, 0X1E03);} else { sendCommand(CMD_PLAY_W_VOL, 0X1E05);}
        //sendCommand(CMD_RAND,randTrackNo);
        //Serial.print("Playing random track:");
        //Serial.println(randTrackNo);        
        delay(2000);
        
        // turn LED on:
        //Serial.println("If Button STate HIGH" + buttonState);
        digitalWrite(ledPin, HIGH);
        //Serial.println(" Arm LED is " + ledPin);

       
        //play firing sound
        randTrackNo = random(4, 6);
        if (randTrackNo == 4) { sendCommand(CMD_PLAY_W_VOL, 0X1E04);} else { sendCommand(CMD_PLAY_W_VOL, 0X1E02);}
        //sendCommand(CMD_RAND,randTrackNo);
        //Serial.print("Playing random track:");
        //Serial.println(randTrackNo);
        delay(2000);
        digitalWrite(ledPin, LOW);
        //Serial.println(" Arm LED is " + ledPin);

        sendCommand(CMD_PLAY_W_VOL, 0X1E01);//play the first song with volume 30 class
        

        /*
         * recoil
         */
        //tiltServo.write(y_pos[i] - recoil);  
        //delay(500);
        //tiltServo.write(y_pos[i]);  

        /*
         * LED Board  Hit or Miss check  then light up
         */
        if(hits[i]){
            shipstatus[hits[i]]++;
            strip.setPixelColor(i, strip.Color(255, 70, 0));
            strip.show();
            if(shipstatus[hits[i]] == shiplength[hits[i]]) {
                colorWipe(strip.Color(255, 0, 0), 250, hits[i]); // Red
              }
          } else {
            strip.setPixelColor(i, strip.Color(255, 255, 255));
            strip.show();
          }
        delay(1000);  


        //Servos return to the center position
        tiltServo.write(75);    
        panServo.write(18);
        delay(1000);
        //Serial.println("RESET");

        digitalWrite(13,LOW);  // LED Strip off

        
        Serial.read(); // read Serial
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
  //Serial.println("Inside sendCommand  playing ");
  //Serial.println(dat);
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

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait, int ship) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(hits[i] == ship) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    }
  }
}
