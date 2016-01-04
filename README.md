# sunrise-simulator
After reading about ambient light alarm clocks, I thought I'd take a stab at making one. 

Here's the Arduino sketch I wrote that functions as an alarm clock. 

# TODO:
~~Remove looping~~

Add python code for starting and stopping 

# Configuration
This sketch assumes you have an RGB (non-addressable) strip hooked up to pins 9 (red), 10 (green), and 11 (blue).

It also requires you to connect via serial to send the commands "on" or "off" to control the sunrise. Since the Arduino has no real time clock, I'm employing the help of my Raspberry Pi and a cron job.
