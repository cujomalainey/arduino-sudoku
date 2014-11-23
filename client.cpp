#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#define PIN_SHIELD            6

#define NUMPIXELS_SHIELD      40

#define SHIELD_DEPTH          8
#define SHIELD_WIDTH          5

Adafruit_NeoPixel shield = Adafruit_NeoPixel(NUMPIXELS_SHIELD, PIN_SHIELD, NEO_GRB + NEO_KHZ800);

uint32_t colors[9];

uint8_t focus_x,focus_y;
uint16_t last_potentionmeter_level;

void write_pixel(uint8_t x, uint8_t y, uint8_t color_id)
{

}

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
}

void loop() {
  if (Serial1.available() > 0 && Serial1.read() == 'C')
  {
    char command = (char)Serial1.read();
    if (command == 'W')
    {
      // write pixel
    }
    else if (command == 'E')
    {
      //erase all pixels
      for (int i = 0; i < NUMPIXELS_SHIELD; i++)
      {
        shield.setPixelColor(i, 0);
      }
      shield.show();
    }
  }
}