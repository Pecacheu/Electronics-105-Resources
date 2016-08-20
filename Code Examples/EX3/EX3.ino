#include <Adafruit_NeoPixel.h>

#define LEDPIN 2
#define POTPIN A0
#define LENGTH 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, LEDPIN, NEO_GRB + NEO_KHZ800);

//Global Variables:
double DIV = 1;
int cycleCount = 0;
byte oldPotValue = 127;
int cycleBreakCount = 0;
int timeToNextUpdate = 0;
byte updateSpeed = 15;
byte patternNumber = 0;

void setup() {
  strip.begin();
  setBrightness(200);
}

void loop() {
  //Display Current Pattern:
  if(!cycleBreakCount) {
    int cycleMax; //Each patern returns it's duration, which
    //is used to determine when to move on to the next one.
    if(patternNumber < 3) cycleMax = rainbowCycle_NonLoop();
    else if(patternNumber < 6) cycleMax = theaterChaseRainbow_NonLoop();
    else if(patternNumber <= 8) cycleMax = RepeatedColorWipe_NonLoop();
    
    //Increment Pattern Counter:
    if(cycleCount >= cycleMax) {
      patternNumber++; cycleCount = 0;
      if(patternNumber > 8) patternNumber = 0;
    }
  }
  
  //Read Potentiometer Value:
  unsigned int potRaw = analogRead(POTPIN);
  byte potValue = mapValues(potRaw, 0, 1023, 0, 255);
  
  //Check If New Value is Different:
  if(abs(potValue - oldPotValue) >= 6) {
    //Display New Pot Value on Light Ring:
    int pixelsToLight = mapValues(potValue, 255, 0, 0, strip.numPixels());
    for(int i=0; i<strip.numPixels(); i++) {
      if(i < pixelsToLight) strip.setPixelColor(i, strip.Color(64/DIV, 255/DIV, 16/DIV));
      else strip.setPixelColor(i, 0);
    } strip.show();
    
    //Set Brightness to Pot Value:
    //setBrightness(-potValue+255);
    
    //Set Speed to Pot Value:
    updateSpeed = mapValues(potValue, 0, 255, 1, 30);
    timeToNextUpdate = updateSpeed;
    
    //Update Variables:
    oldPotValue = potValue;
    
    //Hold Display For 1 Second:
    //(5ms long cycle * 200 cycles = 1000ms = 1 second)
    cycleBreakCount = 200;
  }
  
  if(cycleBreakCount) cycleBreakCount--; //If holding value display, advance timer variable.
  delay(5);
}

//Wait for a button to be released for an amount of time:
void waitForButtonRelease(byte pinNum, unsigned int time) {
  unsigned int bCount = 0;
  while(bCount < time) {
    delay(1); bCount++;
    if(digitalRead(pinNum) == HIGH) bCount = 0;
  }
}

//Set Brightness (from 0 to 255) of lights.
void setBrightness(byte brightness) {
  DIV = double(-brightness+256)/8.0;
  if(DIV < 1) DIV = 1;
}

//Non-looping version of rainbowCycle function, using a global counter:
int rainbowCycle_NonLoop() {
  //Check if it's time to run next update:
  if(timeToNextUpdate == 0) {
    //Check if cycle counter needs to be reset:
    if(cycleCount >= 256) cycleCount = 0;
    //Run light update:
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(i * 255 / strip.numPixels() + cycleCount));
    } strip.show();
    //Increment cycle counter, reset time-to-next-update:
    cycleCount++;
    timeToNextUpdate = updateSpeed;
  }
  //Increment time-to-update:
  timeToNextUpdate--;
  return 256;
}


//Non-looping version of theaterChaseRainbow function, using a global counter:
int theaterChaseRainbow_NonLoop() {
  //Check if it's time to run next update:
  if(timeToNextUpdate == 0) {
    //Check if cycle counter needs to be reset:
    if(cycleCount >= 256) cycleCount = 0;
    //Turn off lights, but don't update changes:
    for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }
    //Turn on every 3rd light, update changes:
    int offset = cycleCount % 2;
    for (int i=0; i < strip.numPixels(); i+=2) {
      strip.setPixelColor(i+offset, Wheel((i+cycleCount) % 255) );
    } strip.show();
    //Increment cycle counter, reset time-to-next-update:
    cycleCount++;
    timeToNextUpdate = updateSpeed;
  }
  //Increment time-to-update:
  timeToNextUpdate--;
  return 256;
}

//Color table for RepeatedColorWipe:
int colorSeq[][3] = {
  {255, 32, 24},
  {128, 64, 196},
  {212, 255, 24},
  {64, 255, 128},
  {232, 128, 0},
  {16, 64, 255}
};

//Non-looping and repeating version of ColorWipe function, using a tabe and a global counter:
int RepeatedColorWipe_NonLoop() {
  //Check if it's time to run next update:
  if(timeToNextUpdate == 0) {
    //Check if cycle counter needs to be reset:
    if(cycleCount >= sizeof(colorSeq) / sizeof(colorSeq[0]) * strip.numPixels()) cycleCount = 0;
    //Calculate led number and color:
    unsigned int led = cycleCount % strip.numPixels();
    unsigned int currSeq = cycleCount / strip.numPixels();
    uint32_t color = strip.Color(colorSeq[currSeq][0]/DIV, colorSeq[currSeq][1]/DIV, colorSeq[currSeq][2]/DIV);
    //Calculate last used color:
    unsigned int prevSeq = currSeq - 1; if(currSeq == 0) prevSeq = (sizeof(colorSeq)/sizeof(colorSeq[0])) - 1;
    uint32_t prevColor = strip.Color(colorSeq[prevSeq][0]/DIV, colorSeq[prevSeq][1]/DIV, colorSeq[prevSeq][2]/DIV);
    //Run light update:
    for(int i=0; i<strip.numPixels(); i++) {
      if(i <= led) strip.setPixelColor(i, color);
      else strip.setPixelColor(i, prevColor);
    } strip.show();
    //Increment cycle counter, reset time-to-next-update:
    cycleCount++;
    timeToNextUpdate = updateSpeed;
  }
  //Increment time-to-update:
  timeToNextUpdate--;
  return sizeof(colorSeq) / sizeof(colorSeq[0]) * strip.numPixels();
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color((255 - WheelPos * 3)/DIV, 0, (WheelPos * 3)/DIV);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, (WheelPos * 3)/DIV, (255 - WheelPos * 3)/DIV);
  } else {
   WheelPos -= 170;
   return strip.Color((WheelPos * 3)/DIV, (255 - WheelPos * 3)/DIV, 0);
  }
}

//Pecacheu's ultimate unit translation formula:
//This Version -- Bounds Checking: NO, Rounding: YES-FLOAT, Max/Min Switching: NO
int mapValues(float input, float minIn, float maxIn, float minOut, float maxOut) {
  float result = ((input-minIn)/(maxIn-minIn)*(maxOut-minOut))+minOut; return round(result);
}
