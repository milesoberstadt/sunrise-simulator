class ColorDirective(object):
	targetColor = "0x000000"
	targetBrightness = 1.0
	cycleTime = 1.0

	def __init__(self, color, brightness, time):
		self.targetColor = color
		self.targetBrightness = brightness
		self.cycleTime = time