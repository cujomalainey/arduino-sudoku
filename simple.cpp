// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN_SHIELD            6
#define PIN_STICK_1           50
#define PIN_STICK_2           51

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS_SHIELD      40

#define NUMPIXELS_STICK       8
#define SHIELD_DEPTH          8
#define SHIELD_WIDTH          5

const int amplitude = 10;

Adafruit_NeoPixel shield = Adafruit_NeoPixel(NUMPIXELS_SHIELD, PIN_SHIELD, NEO_GRB + NEO_KHZ800);

typedef struct {
  uint8_t device;
  uint8_t pin;
  uint8_t led;
  uint8_t color_id;
  uint8_t locked;
} pixel;

pixel board[9][9];
uint32_t colors[9];

uint8_t focus_x,focus_y;

void write_pixel(uint8_t x, uint8_t y, uint8_t color_id)
{
  if (board[x][y].device)
  {
    //write local
  }
  else
  {
    //write remote
  }
}

uint8_t verify()
{

}

void check_focus()
{
  // check if thumbstick wanting to move
  // if yes, make sure old forcus pixel is set back to color
  // start flashing new pixel
}

void setup() {
  shield.begin(); // This initializes the NeoPixel library.
  randomSeed(6798);
  Serial.begin(9600);

  // WHITE IS RESERVED FOR ERRORS
  colors[0] = shield.Color(20,2,12); //pink
  colors[1] = shield.Color(20,0,0); // Red
  colors[2] = shield.Color(20,13,0); // orange
  colors[3] = shield.Color(20,20,0); // yellow
  colors[4] = shield.Color(0,20,0); // green
  colors[5] = shield.Color(0,20,20); // cyan
  colors[6] = shield.Color(0,0,20); // blue
  colors[7] = shield.Color(20,0,20); // purple
  colors[8] = shield.Color(10,10,10);  // grey

  // Define where all LEDS are
  // Define primary shield
  for (int width = 0; width < SHIELD_WIDTH; width++)
  {
    for (int depth = 0; depth < SHIELD_DEPTH; depth++)
    {
      board[width][depth].device = 0;
      board[width][depth].pin = 6;
      board[width][depth].led = depth + SHIELD_DEPTH*width;
    }
  }
  for (int width = SHIELD_WIDTH; width < 9; width++)
  {
    for (int depth = 0; depth < SHIELD_DEPTH; depth++)
    {
      board[width][depth].device = 1;
      board[width][depth].pin = 6;
      board[width][depth].led = depth + SHIELD_DEPTH*(width - SHIELD_WIDTH);
    }
  }
}

void loop() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  uint32_t color = shield.Color(255,255,255);
  shield.setPixelColor(0, color);
  shield.show();
}
