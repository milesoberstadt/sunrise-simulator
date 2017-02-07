# sunrise-simulator
After reading about ambient light alarm clocks, I thought I'd take a stab at making one. 

Here's the Arduino sketch I wrote that functions as an alarm clock. 


# Configuration
This sketch assumes you have an RGB (non-addressable) strip hooked up to pins 9 (red), 10 (green), and 11 (blue).

I have now moved to a dumber Arduino setup which only reads serial, converts the sent value, and displays. 
This allows for a more flexible python client to control the lights and possibly integrate with other stuff. 

# Compiling
Make sure you have the [Arduino IDE](https://www.arduino.cc/en/Main/Software) installed! 
If you're using Linux, you can run the compile script inside the sketch folder to upload the sketch to your Arduino.

# Running the client
This project uses python2.7, but you should be able to use other versions. `./startSun.sh` will run the sunrise, `./stopSun.sh` will set the Arduino's color to "000000".
