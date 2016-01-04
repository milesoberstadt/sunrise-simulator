#include <math.h>

class Cycle
{
  public: 
    int cycleLength;
    float cycleBrightness;
    int redTargetColor;
    int greenTargetColor;
    int blueTargetColor;
};

int red = 9;           // the PWM pin the RED LED is attached to
int green = 10;        // the PWM pin the GREEN LED is attached to
int blue = 11;         // the PWM pin the BLUE LED is attached to

float rBrightness = 0;
int startRed = 0; // Keep track of the starting color (from the previous cycle)
float gBrightness = 0;
int startGreen = 0; // Keep track of the starting color (from the previous cycle)
float bBrightness = 0;
int startBlue = 0; // Keep track of the starting color (from the previous cycle)

float currentBrightness = 0; // Keep track of the current brightness so we can go to the target one...
float startBrightness = 0; // Brightness at the beginning of this cycle/end of last cycle
// A real time sunrise is ~75 minutes, which is what a multiplier of 1 gives you. I want a 25 minute one, so I'm using 3...
int speedMultiplier = 3;

boolean bRunning = false;

// Decides if we want to start over when done, might be useful for someone eles' project
boolean bLoop = false;

#define numCycles 10
Cycle *cycles[numCycles];
int currentCycle = 0;

// Runs at the beginning of the light show.
void setup() {
  // Setup serial input...
  Serial.begin(9600);
  // Setup our output pins
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);

  // Fill our array with cycle objects
  for (int i=0; i<numCycles; i++){
    cycles[i] = new Cycle();
  }

  // Actual definition of our cycle paterns
  cycles[0] = new Cycle();
  cycles[0]->cycleLength = 7.5 * 60;
  cycles[0]->cycleBrightness = 0.1;
  cycles[0]->redTargetColor = 41;
  cycles[0]->greenTargetColor = 39;
  cycles[0]->blueTargetColor = 39;

  cycles[1] = new Cycle();
  cycles[1]->cycleLength = 7.5 * 60;
  cycles[1]->cycleBrightness = 0.1;
  cycles[1]->redTargetColor = 53;
  cycles[1]->greenTargetColor = 52;
  cycles[1]->blueTargetColor = 49;

  cycles[2] = new Cycle();
  cycles[2]->cycleLength = 7.5 * 60;
  cycles[2]->cycleBrightness = 0.1;
  cycles[2]->redTargetColor = 158;
  cycles[2]->greenTargetColor = 129;
  cycles[2]->blueTargetColor = 95;

  cycles[3] = new Cycle();
  cycles[3]->cycleLength = 7.5 * 60;
  cycles[3]->cycleBrightness = 0.2;
  cycles[3]->redTargetColor = 245;
  cycles[3]->greenTargetColor = 184;
  cycles[3]->blueTargetColor = 120;

  cycles[4] = new Cycle();
  cycles[4]->cycleLength = 7.5 * 60;
  cycles[4]->cycleBrightness = 0.3;
  cycles[4]->redTargetColor = 217;
  cycles[4]->greenTargetColor = 167;
  cycles[4]->blueTargetColor = 113;

  cycles[5] = new Cycle();
  cycles[5]->cycleLength = 7.5 * 60;
  cycles[5]->cycleBrightness = 0.4;
  cycles[5]->redTargetColor = 176;
  cycles[5]->greenTargetColor = 144;
  cycles[5]->blueTargetColor = 108;

  cycles[6] = new Cycle();
  cycles[6]->cycleLength = 7.5 * 60;
  cycles[6]->cycleBrightness = 0.5;
  cycles[6]->redTargetColor = 164;
  cycles[6]->greenTargetColor = 140;
  cycles[6]->blueTargetColor = 113;

  cycles[7] = new Cycle();
  cycles[7]->cycleLength = 7.5 * 60;
  cycles[7]->cycleBrightness = 0.8;
  cycles[7]->redTargetColor = 249;
  cycles[7]->greenTargetColor = 160;
  cycles[7]->blueTargetColor = 56;

  cycles[8] = new Cycle();
  cycles[8]->cycleLength = 7.5 * 60;
  cycles[8]->cycleBrightness = 0.9;
  cycles[8]->redTargetColor = 208;
  cycles[8]->greenTargetColor = 154;
  cycles[8]->blueTargetColor = 67;

  cycles[9] = new Cycle();
  cycles[9]->cycleLength = 7.5 * 60;
  cycles[9]->cycleBrightness = 1.0;
  cycles[9]->redTargetColor = 255;
  cycles[9]->greenTargetColor = 221;
  cycles[9]->blueTargetColor = 126;

}

// loop runs until our display is done
void loop() {

  // Initially, we need to wait for a command to run...
  checkForSerialData();

  // If we shouldn't be running, do nothing.
  if (!bRunning) {
	  return;
  }

  // set the brightness of all the things
  analogWrite(red, floor(rBrightness*currentBrightness));
  analogWrite(blue, floor(bBrightness*currentBrightness));
  analogWrite(green, floor(gBrightness*currentBrightness));
  
  // Update to get closer to our current cycle
  rBrightness = mergeColor(rBrightness, startRed, cycles[currentCycle]->redTargetColor, cycles[currentCycle]->cycleLength);
  gBrightness = mergeColor(gBrightness, startGreen, cycles[currentCycle]->greenTargetColor, cycles[currentCycle]->cycleLength);
  bBrightness = mergeColor(bBrightness, startBlue, cycles[currentCycle]->blueTargetColor, cycles[currentCycle]->cycleLength);

  // Update brightness...
  currentBrightness = mergeColor(currentBrightness, startBrightness, cycles[currentCycle]->cycleBrightness, cycles[currentCycle]->cycleLength);
  
  // See if we're done with our cycle...
  if (cycles[currentCycle]->redTargetColor == rBrightness && cycles[currentCycle]->greenTargetColor == gBrightness && cycles[currentCycle]->blueTargetColor == bBrightness){
	  Serial.println("Cycle complete!");
	  // Set the start colors...
	  startRed = cycles[currentCycle]->redTargetColor;
	  startGreen = cycles[currentCycle]->greenTargetColor;
	  startBlue = cycles[currentCycle]->blueTargetColor;
	  startBrightness = cycles[currentCycle]->cycleBrightness;
	  // Reset the cycle count
	  currentCycle = currentCycle+1;

	// Here's where we detect if we've finished...
	if (currentCycle >= numCycles) {
		if (bLoop)
			// Restart
			currentCycle = 0;
		else
			// Otherwise stay in our current cycle so nothing changes, we'll wait for the kill signal
			currentCycle = numCycles - 1;

	}
	Serial.println("Starting cycle " + String(currentCycle));
  }

  // wait for 1000 milliseconds / speed multiplier to process our colors
  delay(1000/speedMultiplier);
}

float mergeColor(float currentColorAmount, float startColor,  float targetColor, int cycleLength){
  float returnColor = currentColorAmount;
  float changeAmount = (float(targetColor - startColor) / float(cycleLength));
  
  /*Serial.println("Before:");
	Serial.println(changeAmount);
	Serial.println(returnColor);
  */
  
  if (changeAmount>0){ // Add on the condition that we don't go over
    returnColor = min((returnColor + changeAmount), targetColor);
  }
  else if (changeAmount<0){ // Subtract on the condition that we don't go under
    returnColor = max((returnColor + changeAmount), targetColor);
  }
  
  /*Serial.println("After:");
	Serial.println(returnColor);
  */
  return returnColor;
}

void checkForSerialData() {
	if (Serial.available() > 0) {
		//Read incoming data
		String incomingString;
		incomingString = Serial.readString();
		if (incomingString == "on") {
			Serial.println("Turning on!");
			bRunning = true;
		}
		else if (incomingString == "off") {
			Serial.println("Turning off!");
			bRunning = false;
			reset();
		}
	}
}

void reset() {
	// Kill the lights!
	analogWrite(red, 0);
	analogWrite(blue, 0);
	analogWrite(green, 0);

	// Go back to the beginning of the first cycle.
	currentBrightness = 0;
	currentCycle = 0;
	rBrightness = 0;
	gBrightness = 0;
	bBrightness = 0;
}
