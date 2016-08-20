#include <TimerOne.h>

//Pin Numbers:
#define PinNumA 3
#define PinNumB 4
#define ColorR  5
#define ColorG  6
#define ColorB  9

#define ECYCLESPEED 20 //20 x 10 = 200ms Timeout

//Encoder Read Vars:
int clicks = 0;
boolean PinA, PinB,
readStep = false;

//Encoder Color Cycle Vars:
volatile int timer = ECYCLESPEED;
volatile uint16_t eWheelPos = 0;

void setup() {
  //Setup Encoder Read Pins:
  pinMode(PinNumA, INPUT);
  digitalWrite(PinNumA, HIGH); //Enable pull-up.
  pinMode(PinNumB, INPUT);
  digitalWrite(PinNumB, HIGH); //Enable pull-up.
  pinMode(ColorR, OUTPUT);
  pinMode(ColorG, OUTPUT);
  pinMode(ColorB, OUTPUT);
  
  //Init Encoder Color Cycle Timer:
  Timer1.initialize(10000); //Interval in us.
  Timer1.attachInterrupt(encoderTimer);
}

void loop() {
  readEncoder(); //Call this as often as possible in loop to accuratly read encoder.
}

void readEncoder() {
  encoderReadPins();
  if(PinA && !PinB) { //PinA is on and pinB is off:
    btnReleaseINV(PinNumA, 1);
    for(byte i=0; i<5; i++) { //Wait 5ms for pinB to turn on:
      encoderReadPins();
      if(PinB) { readStep = true; btnReleaseINV(PinNumB, 2); break; } delay(1);
    }
    if(readStep) { clicks++; encoderMoved(true); } else encoderReadError(true);
    readStep = false;
  } else if(PinB && !PinA) { //PinB is on and pinA is off:
    btnReleaseINV(PinNumB, 1);
    for(byte i=0; i<5; i++) { //Wait 5ms for pinA to turn on:
      encoderReadPins();
      if(PinA) { readStep = true; btnReleaseINV(PinNumA, 2); break; } delay(1);
    }
    if(readStep) { clicks--; encoderMoved(false); } else encoderReadError(false);
    readStep = false;
  }
}

void encoderReadPins() {
  PinA = !digitalRead(PinNumA);
  PinB = !digitalRead(PinNumB);
}

void encoderMoved(boolean dir) {
  //This code runs when the encoder is moved.
  //The value is stored in the 'clicks' variable.
  Serial.println(clicks);
  digitalWrite(13, !digitalRead(13));
}

void encoderReadError(boolean dir) {
  //This code runs when a read error occurs. You can just ignore these errors.
  Serial.print("READ ERROR: DIR"); Serial.print(dir?"++":"--");
  Serial.print(", RS = "); Serial.println(readStep);
}

void btnReleaseINV(byte pinNum, unsigned int time) {
  unsigned int bCount = 0;
  while(bCount < time) {
    delay(1); bCount++;
    if(digitalRead(pinNum) == LOW) bCount = 0; //INVERSED!
  }
}

void encoderTimer() {
  if(timer >= ECYCLESPEED) {
    encoderColorFade(); timer = 0;
  } else timer++;
}

void encoderColorFade() {
  byte r, g, b;
  if(eWheelPos < 256) {
    r = eWheelPos % 256; g = 0; b = (255-eWheelPos % 256);
  } else if(eWheelPos < 512) {
    r = (255-eWheelPos % 256); g = eWheelPos % 256; b = 0;
  } else if(eWheelPos < 768) {
    r = 0; g = (255-eWheelPos % 256); b = eWheelPos % 256;
  }
  analogWrite(ColorR, 255-r); analogWrite(ColorG, 255-g); analogWrite(ColorB, 255-b);
  if(eWheelPos >= 768) eWheelPos = 0; else eWheelPos++;
}
