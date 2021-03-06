#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#define PIN_SHIELD            6

#define NUMPIXELS_SHIELD      40

#define SHIELD_DEPTH          8
#define SHIELD_WIDTH          5

Adafruit_NeoPixel shield = Adafruit_NeoPixel(NUMPIXELS_SHIELD, PIN_SHIELD, NEO_GRB + NEO_KHZ800);

uint32_t colors[11];

void setup() {
  shield.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);
  Serial1.begin(9600);

  // WHITE IS RESERVED FOR ERRORS
  colors[0] = shield.Color(20,2,12);   // Pink
  colors[1] = shield.Color(20,0,0);    // Red
  colors[2] = shield.Color(11,5,2);    // Brown
  colors[3] = shield.Color(20,10,0);   // Orange
  colors[4] = shield.Color(20,20,0);   // Yellow
  colors[5] = shield.Color(0,20,0);    // Green
  colors[6] = shield.Color(0,20,20);   // Cyan
  colors[7] = shield.Color(0,0,20);    // Blue
  colors[8] = shield.Color(10,0,20);   // Purple
  colors[9] = shield.Color(20,20,20);  // White
  colors[10] = shield.Color(0,0,0);    // OFF

  // clear grid on start
  for (int i = 0; i < NUMPIXELS_SHIELD; i++)
  {
    shield.setPixelColor(i, colors[10]);
  }
  // must be called to push changes to the grid.
  shield.show();
}

void loop() {
  // check if data in the buffer and is valid
  if (Serial1.available() > 0 && (char)Serial1.read() == 'C')
  {
    // wait for more data
    while (Serial1.available() < 1){}

    // If next data is valid continue, otherwise return
    if ((char)Serial1.peek() == 'W')
    {
      // write pixel to grid

      // pop next buffer
      Serial1.read();

      // get data from buffer
      uint8_t led;
      uint8_t color_id;
      while (Serial1.available() < 1){}
        led = Serial1.read() - 1;
        while (Serial1.available() < 1){}
        color_id = Serial1.read() - 1;
        shield.setPixelColor(led, colors[color_id]);
        shield.show();
    }
    else if ((char)Serial1.peek() == 'E')
    {
      //erase all pixels

      // pop next buffer
      Serial1.read();

      // clear all LEDs
      for (int i = 0; i < NUMPIXELS_SHIELD; i++)
      {
        shield.setPixelColor(i, colors[10]);
      }
      shield.show();
    }
  }
}