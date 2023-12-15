# Stairway-Stopwatch
![image](https://github.com/jareddilley/Stairway-Stopwatch/blob/main/Media/Demo-video.gif)

Racing up and down the stairs used to be for kids, not anymore. As the holidays approach I have created a stopwatch so can turn racing up and down the stairs into a sport. The 3D printed system is wireless and uses motion sensing for ease of use. This would be a great build for you and your roommates or even your kids. Below are the schematics for both the receiver and transmitter:

![image](https://github.com/jareddilley/Stairway-Stopwatch/blob/main/Media/schematic-receiver.PNG)
![image](https://github.com/jareddilley/Stairway-Stopwatch/blob/main/Media/schematic-transmitter.PNG)

# Displays
![image](https://github.com/jareddilley/Stairway-Stopwatch/blob/main/Media/Displays.png)

The main stopwatch display is a MAX7219 Dot Matrix Module for Arduino Microcontroller 4-in-1 Display. It comes in a red and green variety. The display is updated easily by simply sending a string value to the display. The default font did not fit neatly in the full display so I used a Font.h file to alter the font of the displayed numbers. For the best time tracker, I used an I2C Serial LCD Module 16x2 Display. This display uses a cursor to display the values so when updating I have to redraw all the letters. 

# Wireless Communication
![image](https://github.com/jareddilley/Stairway-Stopwatch/blob/main/Media/Wireless-com.gif)

The wireless communication took some troubleshooting. I tried the 315Mhz RF Transmitter and Receiver Modules but the range was not enough to reach up to 3 stories and is only one-way transmission. I also tried the NRF24L01+ 2.4GHz Arduino Wireless RF Transceiver but I could not get it to properly work with my Arduino boards. Luckily I was able to land on a solution, the HC-12 433Mhz SI4438 Wireless Serial Port Module. This module says it can do up to 1000 meters, way beyond my needs. It has a simple pin layout to reduce wires and uses simple serial print and read functions to communicate back and forth. 

![image](https://github.com/jareddilley/Stairway-Stopwatch/blob/main/Media/HC-12.JPG)

# Motion sensing
![image](https://github.com/jareddilley/Stairway-Stopwatch/blob/main/Media/Motion-sensor.gif)

Ultrasonic sensor with moving average, transmitter has a ready up button with 2 second delay


