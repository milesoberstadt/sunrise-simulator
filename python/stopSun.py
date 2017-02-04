
#!/usr/bin/python

import serial
import time
from colordirective import ColorDirective
from ColorDisplaySerial import ColorDisplaySerial

cycleLength = 1

# Add color directives for this cycle

displayList = [
	ColorDirective("000000", 1, cycleLength), 
]

displayManager = ColorDisplaySerial()
displayManager.displayList = displayList

displayManager.startColorDisplay()
