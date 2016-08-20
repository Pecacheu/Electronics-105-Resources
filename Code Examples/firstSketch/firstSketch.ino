//This is a comment. It's for humans to read, and it has ABSOLUTELY NO effect on the code.
//To make a comment, just type "//" and the rest of the line will be ignored by the Arduino.

#define LEDPIN 13 //Make a constant called LEDPIN and set it to 13.
//A constant is like a variable, but it's value never changes.

//This is how you would make LEDPIN a variable instead:
//int LEDPIN = 13;
//(Don't worry, that code is also "commented out", so it won't do anything.)

//Then, you could change the value of LEDPIN at any time like this:
//LEDPIN = 5;
//You can even do math:
//int math_thing = (LEDPIN + 5) / 2
//(Variable names like "math thing" can't have spaces, so just use "_" instead.)

//By the way, that "int" at the start defines the variable type.
//There are many type of variables, but "int" is the most common.
//"int" stands for interger and it can be a positive or negative number, with a maximum of around 3 million.
//"String" is another common variable type, and you can probably tell what that does.

void setup() {
  //This code only runs once, when the Arduino is turned on.
  pinMode(LEDPIN, OUTPUT); //Set the pin to an OUTPUT (the default is INPUT).
}

void loop() {
  //After setup finishes, this code will loop over and over.
  digitalWrite(LEDPIN, HIGH); //DigitalWrite changes the state of the pin. HIGH is on, LOW is off.
  
  delay(1000); //The delay function waits for a number of mili-seconds before . 1000 ms = 1 second.
  
  digitalWrite(LEDPIN, LOW); //Now we turn pin number LEDPIN off.
  
  delay(1000); //Wait one more second before looping back to the start.
}
