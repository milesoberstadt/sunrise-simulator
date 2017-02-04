# You have to be fucking joking me
from __future__ import division
# That makes division not convert to ints

import serial
import time

class ColorDisplaySerial(object):

	debugMode = True
	ser = ""
	displayList = []
	bLoop = False
	directiveStartColor = "000000"
	currentColor = "000000"
	directiveStartBrightness = 0.0
	currentBrightness = 0.0
	currentTargetDirectiveIndex = 0
	colorStepsPerSecond = 10
	currentStepNumber = 1

	def hexColorToIntRed(self, hexColorString):
		return int(hexColorString[0:2], 16)

	def hexColorToIntGreen(self, hexColorString):
		return int(hexColorString[2:4], 16)

	def hexColorToIntBlue(self, hexColorString):
		return int(hexColorString[4:6], 16)

	def colorIntsToHex(self, r, g, b):
		return '%02x%02x%02x' % (r, g, b)

	def processDisplayLoop(self):
		while(True):
			# Detect the end of the loop
			if (self.currentTargetDirectiveIndex == len(self.displayList)):
				if (self.debugMode == True):
					print("Finished")
				# We'll either exit or loop
				if (self.bLoop == True):
					self.currentTargetDirectiveIndex = 0
					self.currentStepNumber = 1
					time.sleep(1.0/self.colorStepsPerSecond)
				else:
					# Stop the display by sending black...
					self.ser.write("000000;")
					break

			targetColor = self.displayList[self.currentTargetDirectiveIndex].targetColor
			targetBrightness = self.displayList[self.currentTargetDirectiveIndex].targetBrightness
			#stepBrightness = self.directiveStartBrightness + ((targetBrightness - self.directiveStartBrightness) * (self.currentStepNumber/stepsInDirective))
			# Build the target color by multiplying targetColor by targetBrightness
			tRed = int(self.hexColorToIntRed(targetColor)*targetBrightness)
			tGreen = int(self.hexColorToIntGreen(targetColor)*targetBrightness)
			tBlue = int(self.hexColorToIntBlue(targetColor)*targetBrightness)
			targetColor = self.colorIntsToHex(tRed, tGreen, tBlue)
			

			# If we aren't at the end of a loop
			stepsInDirective = self.displayList[self.currentTargetDirectiveIndex].cycleTime * self.colorStepsPerSecond

			if (self.currentStepNumber < stepsInDirective):
				targetRed = self.hexColorToIntRed(targetColor) 
				targetGreen = self.hexColorToIntGreen(targetColor) 
				targetBlue = self.hexColorToIntBlue(targetColor) 

				oRed = self.hexColorToIntRed(self.directiveStartColor);
				oGreen = self.hexColorToIntGreen(self.directiveStartColor);
				oBlue = self.hexColorToIntBlue(self.directiveStartColor);


				# New val is the original val + the percent of the differece between the original val and the target val
				stepRed = int(oRed + ((targetRed-oRed)*(self.currentStepNumber/stepsInDirective)))
				stepGreen = int(oGreen + ((targetGreen-oGreen)*(self.currentStepNumber/stepsInDirective)))
				stepBlue = int(oBlue + ((targetBlue-oBlue)*(self.currentStepNumber/stepsInDirective)))
				#jprint("Current step ",(self.currentStepNumber/stepsInDirective))
				#print("R, G, B", stepRed, stepGreen, stepBlue)

				self.currentColor = self.colorIntsToHex(stepRed, stepGreen, stepBlue)
				self.currentStepNumber = self.currentStepNumber+1
			else:
				if (self.debugMode):
					print("Number of steps elapsed")
				self.currentColor = targetColor	

			if (self.debugMode==True):
				print(self.currentColor+";")
			# Write the new target color
			self.ser.write(self.currentColor+";")

			# Check to see if we're done with this display directive
			if (self.currentColor == targetColor):
				if (self.debugMode):
					print("Step complete, going to next directive")
				self.currentTargetDirectiveIndex = self.currentTargetDirectiveIndex+1
				self.currentStepNumber = 1
				self.directiveStartColor = self.currentColor

			# Recursion!
			time.sleep(1.0/self.colorStepsPerSecond)

	def startColorDisplay(self):	
		self.ser = serial.Serial('/dev/ttyACM0', 9600)
		time.sleep(2)
		#print ser.portstr

		startTime = time.time()
		self.processDisplayLoop()
		endTime = time.time()
		if (self.debugMode==True):
			print(endTime-startTime)

		#time.sleep(5)
		self.ser.close()
