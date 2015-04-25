#include <Adafruit_NeoPixel.h>

#define PIN 2
#define LENGTH 16
byte BRIGHTNESS = 200;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, PIN, NEO_GRB + NEO_KHZ800);

double DIV = double(-BRIGHTNESS+256)/8.0;

void setup() {
  if(DIV < 1) DIV = 1;
  Serial.begin(9600); //Setup USB Serial Port
  strip.begin(); //Setup LED Strip
}

void loop() {
  //Read Potentiometer Value:
  int potRaw = analogRead(A0);
  byte potValue = mapValuesRound(potRaw, 0, 1010, 0, 255);
  
  //Display New Pot Value on Light Ring:
  int pixelsToLight = mapValuesRound(potValue, 255, 0, 0, strip.numPixels());
  
  for(int i=0; i<strip.numPixels(); i++) {
    if(i < pixelsToLight) strip.setPixelColor(i, strip.Color(64/DIV, 255/DIV, 16/DIV));
    else strip.setPixelColor(i, strip.Color(255/DIV, 16/DIV, 64/DIV));
  } strip.show();
  
  //Set Brightness to Pot Value:
  BRIGHTNESS = -potValue+255;
  DIV = double(-BRIGHTNESS+256)/8.0;
  if(DIV < 1) DIV = 1;
  
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
  
  delay(20);
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
