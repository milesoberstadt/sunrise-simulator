
int red = 9;           // the PWM pin the RED LED is attached to
int green = 10;        // the PWM pin the GREEN LED is attached to
int blue = 11;         // the PWM pin the BLUE LED is attached to

int rVal = 0;          // the brightness (0-255) of the red pixel
int gVal = 0;          // the brightness (0-255) of the green pixel
int bVal = 0;          // the brightness (0-255) of the blue pixel

// Runs at the beginning of the light show.
void setup() {
  // Setup serial input...
  Serial.begin(9600);
  // Setup our output pins
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
}


void reset() {
  // Kill the lights!
  analogWrite(red, 0);
  analogWrite(blue, 0);
  analogWrite(green, 0);
}

void parseColors(String incomingString){
  // Get rid of '0x' and convert it to integer
  //int number = (int) strtol( &incomingString[0], NULL, 16);
  long number = strtol( &incomingString[0], NULL, 16);

  // Split them up into r, g, b values
  rVal = number >> 16;
  gVal = number >> 8 & 0xFF;
  bVal = number & 0xFF;
}

void checkForSerialData(){
  if (Serial.available() > 0) {
    char rgb_str[7];
    Serial.readBytesUntil(';', rgb_str, 7);
    String str(rgb_str);
    // Echo it back
    Serial.write(rgb_str);
    parseColors(str);
  }
}

// loop runs until our display is done
void loop() {

  // Initially, we need to wait for a command to run...
  checkForSerialData();

  // set the brightness of all the things
  analogWrite(red, rVal);
  analogWrite(blue, bVal);
  analogWrite(green, gVal);
}
