#include <Adafruit_NeoPixel.h>

#define LEDPIN 2
#define POTPIN A0
#define LENGTH 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, LEDPIN, NEO_GRB + NEO_KHZ800);

double DIV = 1; //Double is probably over-kill for this, but whatever.
//Also, uppercase variables is breaking convention, but it's not the end of the world or anything.

void setup() {
  Serial.begin(9600); //Setup USB Serial Port
  strip.begin(); //Setup LED Strip
  setBrightness(200); //Set Initial Brightness
}

void loop() {
  //Read Potentiometer Value:
  unsigned int potRaw = analogRead(POTPIN);
  byte potValue = mapValues(potRaw, 0, 1023, 0, 255);
  
  int pixelsToLight = mapValues(potValue, 255, 0, 0, strip.numPixels());
  
  //Display New Pot Value on Light Ring:
  for(int i=0; i<strip.numPixels(); i++) {
    if(i < pixelsToLight) strip.setPixelColor(i, strip.Color(64/DIV, 255/DIV, 16/DIV));
    else strip.setPixelColor(i, strip.Color(255/DIV, 16/DIV, 64/DIV));
  } strip.show();
  
  //Set Brightness to Pot Value:
  setBrightness(-potValue+255);
  
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
  
  //We don't need to check the value too
  //frequently, since potentiometers are absolute.
  delay(20);
}

//'byte' is a more readable shortcut to 'uint8_t'
//It's an Arduino-specific thing, so in other C eviorments, just use 'uint8_t'

//Set Brightness (from 0 to 255) of lights.
void setBrightness(byte brightness) {
  Serial.println(brightness);
  DIV = double(-brightness+256)/8.0;
  if(DIV < 1) DIV = 1;
}

//Pecacheu's ultimate unit translation formula:
//This Version -- Bounds Checking: NO, Rounding: YES-FLOAT, Max/Min Switching: NO
int mapValues(float input, float minIn, float maxIn, float minOut, float maxOut) {
  float result = ((input-minIn)/(maxIn-minIn)*(maxOut-minOut))+minOut; return round(result);
}
