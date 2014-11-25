#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include <SD.h>
#include <SPI.h>

#define PIN_SHIELD            6
#define PIN_STICK_1           50
#define PIN_STICK_2           51

#define POTENTIONMETER_PIN    10
#define POTENTIONMETER_CHANGE 10

#define NUMPIXELS_SHIELD      40

#define NUMPIXELS_STICK       8

#define SHIELD_DEPTH          8
#define SHIELD_WIDTH          5

Adafruit_NeoPixel shield = Adafruit_NeoPixel(NUMPIXELS_SHIELD, PIN_SHIELD, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stick1 = Adafruit_NeoPixel(NUMPIXELS_STICK, PIN_STICK_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stick2 = Adafruit_NeoPixel(NUMPIXELS_STICK, PIN_STICK_2, NEO_GRB + NEO_KHZ800);

typedef struct {
  uint8_t device;
  uint8_t shield;
  uint8_t led;
  uint8_t color_id;
  uint8_t locked;
} pixel;

pixel board[9][9];
uint32_t colors[11];

uint8_t focus_x,focus_y;
uint16_t last_potentionmeter_level;

File puzzles;
// file is formated in this fashion
// 1 2 3 4 5 6 7 8 9...81
//
// 1 2 3 4 5 6 7 8 9
// 10 11 12

void write_pixel(uint8_t x, uint8_t y, uint8_t color_id)
{
  if (!board[x][y].device)
  {
    //write local
    if (board[x][y].shield == 0)
    {
      shield.setPixelColor(board[x][y].led, colors[color_id]);
      shield.show();
    }
    else if (board[x][y].shield == 1)
    {
      stick1.setPixelColor(board[x][y].led, colors[color_id]);
      stick1.show();
    }
    else if (board[x][y].shield == 2)
    {
      stick2.show();
    }
  }
  else
  {
    char color[1];
    char led[2] = {(char)board[x][y].led};
    //Serial1.write(command);
    //write remote
    //char str[4] = {"W", led, color_id};
    //Serial1.write(str);
  }
}

// use to display entire grid
void display_grid()
{
  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
    {
      write_pixel(x,y,board[x][y].color_id);
    }
  }
}

uint8_t verify()
{
  // column verification
  for (int x  = 0; x < 9; x++)
  {
    uint8_t items[9] = {0};
    for (int y = 0; y < 9; y++)
    {
      if (items[board[x][y].color_id] == 1)
      {
        return 0;
      }
      else
      {
        items[board[x][y].color_id] = 1;
      }
    }
  }
  // row verification
  for (int y  = 0; y < 9; y++)
  {
    uint8_t items[9] = {0};
    for (int x = 0; x < 9; x++)
    {
      if (items[board[x][y].color_id] == 1)
      {
        return 0;
      }
      else
      {
        items[board[x][y].color_id] = 1;
      }
    }
  }
  // sector verification
  for (int x_sector = 0; x_sector < 3; x_sector++)
  {
    for (int y_sector = 0; y_sector < 3; y_sector++)
    {
      uint8_t items[9] = {0};
      for (int x = 0; x < 3; x++)
      {
        for (int y = 0; y < 3; y++)
        {
          if (items[board[x + x_sector*3][y + y_sector*3].color_id] == 1)
          {
            return 0;
          }
          else
          {
            items[board[x + x_sector*3][y + y_sector*3].color_id] = 1;
          }
        }
      }
    }
  }
  return 1;
}

void check_focus()
{
  // check if thumbstick wanting to move
  // if yes, make sure old forcus pixel is set back to color
  // start flashing new pixel
}

void check_color_change()
{
  if (analogRead(POTENTIONMETER_PIN) - last_potentionmeter_level > POTENTIONMETER_CHANGE || analogRead(POTENTIONMETER_PIN) - last_potentionmeter_level < -POTENTIONMETER_CHANGE)
  {
    last_potentionmeter_level = analogRead(POTENTIONMETER_PIN) - last_potentionmeter_level > POTENTIONMETER_CHANGE ? last_potentionmeter_level + POTENTIONMETER_CHANGE : last_potentionmeter_level - POTENTIONMETER_CHANGE;
    if (!board[focus_x][focus_y].locked)
    {

    }
  }
}

void setup() {
  shield.begin(); // This initializes the NeoPixel library.
  randomSeed(6798);
  Serial.begin(9600);
  Serial1.begin(9600);

  // Clear client
  Serial1.write("CE"); 

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
  

  // Define where all LEDS are

  // Define primary shield
  // Shields address from the top left corner going down, this function maps them to an x,y grid
  for (int width = 0; width < SHIELD_WIDTH; width++)
  {
    for (int depth = 0; depth < SHIELD_DEPTH; depth++)
    {
      board[width][depth].device = 0;
      board[width][depth].shield = 0;
      board[width][depth].led = 32 + depth - SHIELD_DEPTH*width;
    }
  }
  // Define Client Shield
  for (int width = SHIELD_WIDTH; width < 9; width++)
  {
    for (int depth = 0; depth < SHIELD_DEPTH; depth++)
    {
      board[width][depth].device = 1;
      board[width][depth].shield = 0;
      board[width][depth].led = 32 + depth - SHIELD_DEPTH*(width - SHIELD_WIDTH);
    }
  }
  // Define Stick 1
  for (int i = 0; i < 5; ++i)
  {
    board[i][8].device = 0;
    board[i][8].shield = 1;
  }
  board[0][8].led = 0;
  board[1][8].led = 1;
  board[2][8].led = 3;
  board[3][8].led = 4;
  board[4][8].led = 6;
  // Define Stick 2
  for (int i = 5; i < 9; ++i)
  {
    board[i][8].device = 0;
    board[i][8].shield = 2;
  }
  board[5][8].led = 0;
  board[6][8].led = 1;
  board[7][8].led = 3;
  board[8][8].led = 4;
}
  

void loop() {
}