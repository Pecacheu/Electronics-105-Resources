#include <Adafruit_NeoPixel.h>

#define PIN 2
#define LENGTH 16
byte BRIGHTNESS = 200;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, PIN, NEO_GRB + NEO_KHZ800);

//Global Variables:
double DIV = double(-BRIGHTNESS+256)/8.0;
int cycleCount = 0;
byte oldPotValue = 127;
int cycleBreakCount = 0;
int timeToNextUpdate = 0;
byte updateSpeed = 15;
byte patternNumber = 0;

void setup() {
  if(DIV < 1) DIV = 1;
  Serial.begin(9600); //Setup USB Serial Port
  strip.begin(); //Setup LED Strip
}

void loop() {
  //Display Current Pattern:
  if(!cycleBreakCount) {
    int cycleMax;
    if(patternNumber < 3) cycleMax = rainbowCycle_NonLoop();
    else if(patternNumber < 6) cycleMax = theaterChaseRainbow_NonLoop();
    else cycleMax = RepeatedColorWipe_NonLoop();
    
    //Increment Pattern Counter:
    if(cycleCount >= cycleMax) {
      patternNumber++; cycleCount = 0;
      if(patternNumber >= 9) patternNumber = 0;
    }
  }
  
  //Read Potentiometer Value:
  int potRaw = analogRead(A0);
  byte potValue = mapValuesRound(potRaw, 0, 1010, 0, 255);
  
  //Check If New Value is Different:
  if(abs(potValue - oldPotValue) >= 6) {
    //Display New Pot Value on Light Ring:
    int pixelsToLight = mapValuesRound(potValue, 255, 0, 0, strip.numPixels());
    for(int i=0; i<strip.numPixels(); i++) {
      if(i < pixelsToLight) strip.setPixelColor(i, strip.Color(64/DIV, 255/DIV, 16/DIV));
      else strip.setPixelColor(i, 0);
    } strip.show();
    
    //Set Brightness to Pot Value:
    //BRIGHTNESS = -potValue+255;
    //DIV = double(-BRIGHTNESS+256)/8.0;
    //if(DIV < 1) DIV = 1;
    
    //Set Speed to Pot Value:
    updateSpeed = mapValuesFast(potValue, 0, 255, 1, 30);
    timeToNextUpdate = updateSpeed;
    
    //Update Variables:
    oldPotValue = potValue;
    
    //Prevent Rainbow Pattern For 1 Second:
    //(5ms long cycle * 200 cycles = 1000ms = 1 second)
    cycleBreakCount = 200;
  }
  
  /*Serial.println("---------------------------");
  Serial.print("Pot Value: ");
  Serial.println(potValue);
  Serial.print("Raw Value: ");
  Serial.println(potRaw);
  Serial.print("Total Pixels: ");
  Serial.println(strip.numPixels());
  Serial.print("Pixels to Light: ");
  Serial.println(pixelsToLight);
  Serial.println("---------------------------");
  Serial.println("");
  delay(400);*/
  
  if(cycleBreakCount) cycleBreakCount--;
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
      strip.setPixelColor(i+offset, Wheel( (i+cycleCount) % 255) );
    } strip.show();
    //Increment cycle counter, reset time-to-next-update:
    cycleCount++;
    timeToNextUpdate = updateSpeed;
  }
  //Increment time-to-update:
  timeToNextUpdate--;
  return 256;
}

//Non-looping and repeating version of ColorWipe function, using a tabe and a global counter:
int colorSeq[][3] = { {255, 32, 24}, {128, 64, 196}, {212, 255, 24}, {64, 255, 128}, {232, 128, 0}, {16, 64, 255} };
int RepeatedColorWipe_NonLoop() {
  //Check if it's time to run next update:
  if(timeToNextUpdate == 0) {
    //Check if cycle counter needs to be reset:
    if(cycleCount >= sizeof(colorSeq) / sizeof(colorSeq[0]) * strip.numPixels()) cycleCount = 0;
    //Calculate led number and color:
    unsigned int led = cycleCount % strip.numPixels();
    unsigned int currSeq = cycleCount / strip.numPixels();
    uint32_t color = stripColor(colorSeq[currSeq][0], colorSeq[currSeq][1], colorSeq[currSeq][2]);
    //Calculate last used color:
    unsigned int prevSeq = currSeq - 1; if(currSeq == 0) prevSeq = (sizeof(colorSeq)/sizeof(colorSeq[0])) - 1;
    uint32_t prevColor = stripColor(colorSeq[prevSeq][0], colorSeq[prevSeq][1], colorSeq[prevSeq][2]);
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return stripColor(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return stripColor(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return stripColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

//Wrapper to strip.Color function, including brightness:
uint32_t stripColor(byte red, byte green, byte blue) {
  if(BRIGHTNESS == 0) return 0;
  return strip.Color(red/DIV, green/DIV, blue/DIV);
}

//Pecacheu's ultimate unit translation formula:
double mapValues(double input, double minIn, double maxIn, double minOut, double maxOut) {
  char inBound = outOfBounds(input, minIn, maxIn); if(inBound == 1) { input = minIn; } else if(inBound == 2) { input = maxIn; }
  double result = ((input-minIn)/(maxIn-minIn)*(maxOut-minOut))+minOut;
  char outBound = outOfBounds(result, minOut, maxOut); if(outBound == 1) { return minOut; } else if(outBound == 2) { return maxOut; }
  return result;
}
//Rounded version:
int mapValuesRound(double input, double minIn, double maxIn, double minOut, double maxOut) {
  return round(mapValues(input, minIn, maxIn, minOut, maxOut));
} int mapValuesRound(double input, double minIn, double maxIn, double minOut, double maxOut, boolean ceilOrFloor) {
  if(ceilOrFloor) return ceil(mapValues(input, minIn, maxIn, minOut, maxOut));
  else return floor(mapValues(input, minIn, maxIn, minOut, maxOut));
}
//High-Speed Integer version:
int mapValuesFast(float input, int minIn, int maxIn, int minOut, int maxOut) {
  int result = ((input-minIn)/(maxIn-minIn)*(maxOut-minOut))+minOut;
  char outBound = outOfBounds(result, minOut, maxOut, true);
  if(outBound == 1) { return minOut; } else if(outBound == 2) { return maxOut; }
  return result;
}
//Check if value is out-of-bounds, and in which direction:
byte outOfBounds(double input, double a, double b) {
  if(b > a) {
    if(input < a) return 1;
    else if(input > b) return 2;
    return 0;
  } else if(a > b) {
    if(input > a) return 1;
    else if(input < b) return 2;
    return 0;
  } else {
    if(input == a) return 1;
    return 0;
  }
} byte outOfBounds(int input, int a, int b, boolean fast) {
  if(b > a) {
    if(input < a) return 1;
    else if(input > b) return 2;
    return 0;
  } else if(a > b) {
    if(input > a) return 1;
    else if(input < b) return 2;
    return 0;
  } else {
    if(input == a) return 1;
    return 0;
  }
}
