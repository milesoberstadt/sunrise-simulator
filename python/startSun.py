
#!/usr/bin/python

import serial
import time
from colordirective import ColorDirective
from ColorDisplaySerial import ColorDisplaySerial

cycleLength = 7.5 * 60

# Add color directives for this cycle

displayList = [
	ColorDirective("292727", 0.1, cycleLength), 
	ColorDirective("353431", 0.1, cycleLength), 
	ColorDirective("9E815F", 0.1, cycleLength), 
	ColorDirective("F5B878", 0.2, cycleLength), 
	ColorDirective("D9A771", 0.3, cycleLength), 
	ColorDirective("B0906C", 0.4, cycleLength), 
	ColorDirective("A48C71", 0.5, cycleLength), 
	ColorDirective("F9A038", 0.8, cycleLength), 
	ColorDirective("D09A43", 0.9, cycleLength), 
	ColorDirective("FFDD7E", 1.0, cycleLength), 
]

displayManager = ColorDisplaySerial()
displayManager.displayList = displayList

displayManager.startColorDisplay()
