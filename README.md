arduino-sudoku
==============

Arduino LED Sudoku
-----------------------------------------------
Course: CMPUT 274
Assignment: Project
Contributors: Curtis Malainey, Satyen Akolkar 
Date: Nov. 10, 2014

-----------------------------------------------
Acknowledgements: 

The puzzle library was posted on the web here http://www2.warwick.ac.uk/fac/sci/moac/people/students/peter_cock/python/sudoku/ we are simply importing it.
Adafruit provides the Neopixel library here https://github.com/adafruit/Adafruit_NeoPixel and is also included in the project

------------------------------------------------
Description: 

This program is a colour sudoku puzzle. The user is given a random 9by9 pre-loaded puzzle where 9 colours represent the conventional 9 numbers. The user can navigate the puzzle using the DPAD controller and change the value using increment and decrement buttons. When the user is finished they can check if they are correct with a verify button.

Notes on compilation, only one of simple.cpp (the server) or client.cpp can be in the directory during compilation, and a "make clean" is require to switch between the the two programs.

------------------------------------------------ 
Wiring

Breadboard General
 - Positive Bus Strip to Arduino +5V
 - Negative Bus Strip to Arduino GND

Sudoku Controller using 8 button switches
 - All button switches are connected to negative bus at one end the other end connection is specified below
 - DPAD_DOWN to Arduino Pin 12
 - DPAD_LEFT to Arduino Pin 11
 - DPAD_UP to Arduino Pin 10
 - DPAD_RIGHT to Arduino Pin 9
 - DPAD_VERIFY to Arduino Pin 8
 - COLOR_INC to Arduino Pin 7
 - COLOR_DEC to Arduino Pin 4
 - RESET to Arduino RST Pin
 
LCD Screen Wiring
 - GND to breadboard negative bus
 - VCC to breadboard positive bus
 - RESET to Arduino Pin 8
 - D/C to Arduino Pin 7
 - CARD_CS to Arduino Pin 5
 - MOSI to Arduino Pin 51
 - SCK to Arduino Pin 52
 - MISO to Arduino Pin 50
 - LITE to breadboard positive bus
 
LED Wiring
 - Shields built on top of the arduinos therefore GND and VCC are alread connected
 - Shields use PIN 6 on the arduino for DATA
 - LED Sticks are connected to GND and VCC as well
 - LED Sticks data pins are 48 and 49 for each stick

-----------------------------------------------
