#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include <SD.h>
#include <SPI.h>

#define PIN_SHIELD            6
#define PIN_STICK_1           48
#define PIN_STICK_2           49

#define DPAD_DOWN             12
#define DPAD_LEFT             11
#define DPAD_UP               10
#define DPAD_RIGHT            9

#define DPAD_VERIFY           8
#define COLOR_INC             7
#define COLOR_DEC             4

#define NUMPIXELS_SHIELD      40

#define NUMPIXELS_STICK       8

#define SHIELD_DEPTH          8
#define SHIELD_WIDTH          5

const int RND_PIN = 2; 

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS    5  // Chip select line for SD card

Adafruit_NeoPixel shields[3]= { Adafruit_NeoPixel(NUMPIXELS_SHIELD, PIN_SHIELD, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMPIXELS_STICK, PIN_STICK_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUMPIXELS_STICK, PIN_STICK_2, NEO_GRB + NEO_KHZ800)};

typedef struct {
  uint8_t device;
  uint8_t shield;
  uint8_t led;
  uint8_t color_id;
  uint8_t locked;
} pixel;

pixel board[9][9];
uint32_t colors[11];

void write_pixel(uint8_t x, uint8_t y, uint8_t color_id)
{
  if (!board[x][y].device)
  {
    //write local
    shields[board[x][y].shield].setPixelColor(board[x][y].led, colors[color_id]);
    shields[board[x][y].shield].show();
  }
  else
  {
    Serial1.write('C');
    Serial1.write('W');
    Serial.print("W LED:");
    Serial1.write(board[x][y].led + 2);
    Serial.print(board[x][y].led);
    Serial.print(" COLOR:");
    Serial1.write(color_id + 2);
    Serial.println(color_id);
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

uint8_t check_empties(uint8_t * items)
{
  for (int i = 0; i < 9; i++)
  {
    if (!items[i])
    {
        return 1;
    }
  }
  return 0;
}

uint8_t verify_check()
{
  // column verification
  for (int x  = 0; x < 9; x++)
  {
    uint8_t items[9] = {0};
    for (int y = 0; y < 9; y++)
    {
      if (items[board[x][y].color_id] == 1)
      {
        Serial.println(board[x][y].color_id);
        return 0;
      }
      else
      {
        items[board[x][y].color_id] = 1;
      }
    }
    if(check_empties(items))
    {
      return 0;
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
    if(check_empties(items))
    {
      return 0;
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
      if(check_empties(items))
      {
        return 0;
      }
    }
  }
  return 1;
}

void verify()
{
  if (verify_check())
  {
    // TODO MAKE GREEN
  }
  else
  {
    for (int x = 0; x < 9; x++)
    {
      for (int y = 0; y < 9; y++)
      {
        write_pixel(x,y,10);
      }
    }
    for (int i = 0; i < 5; i++)
    {
      for (int x = 0; x < 9; x++)
      {
        write_pixel(x,x,1);
        write_pixel(x,8-x,1);
      }
      delay(500);
      for (int x = 0; x < 9; x++)
      {
        write_pixel(x,x,10);
        write_pixel(x,8-x,10);
      }
      delay(500);
    }
  }
  display_grid();
}

int load_puzzle(char filename[55], int line)
{
  File puzzle_file;
  char buffer[82];
  puzzle_file = SD.open(filename, FILE_READ);
  
  if(puzzle_file != NULL) 
  {
    for(int i = 0; i < line; i++)
    {
      for (int c = 0; c < 82; c++)
      {
         buffer[c] = puzzle_file.read();
      }
    }
    for (int x = 0; x < 81; x++)
    {
      Serial.print(buffer[x]);
    }
    for(int i = 0; i < 9; i++) 
    {
      for(int j = 0; j < 9; j++) 
      {
        if(buffer[j*9 + i] != '.') 
        {
        //parse the next number from the buffer
        sscanf(&buffer[j*9 + i], "%1d", (int*)&board[i][j].color_id);
        //subtract the number by 1 to match colour ids
        board[i][j].color_id--;
        board[i][j].locked = 1;
        }
        //if element is to be blank set colour white (id = 9)
        else
        {
          board[i][j].color_id = 10;
          board[i][j].locked = 0;
        }
      }
    }

    //close the file
    puzzle_file.close();
  }
  //report error if file couldn't be opened
  else
  {
    printf("Could not open file.");
    return 0;
  }
  
  return 1;
}

unsigned int random16bits() {
  unsigned int a = 0;
  for (int i=0; i<16; ++i) {
    a<<=1;
    a+=analogRead(RND_PIN)%2;
    delay(50);
  }
  return a;
}

void setup() {
  // This initializes the NeoPixel library.
  for (int i = 0; i < 3; i++)
  {
    shields[i].begin();
  }

  randomSeed(random16bits());
  Serial.begin(9600);
  Serial1.begin(9600);

  // Initialize SD Card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    while (1) {} // stuff exploded
  }
  Serial.println("");

  //TODO clear buffer for Serial1

  // Clear client
  Serial1.write('C');
  Serial1.write('E'); 
  // TODO TIMEOUT function for client

  // WHITE IS RESERVED FOR ERRORS
  colors[0] = shields[0].Color(20,2,12);   // Pink
  colors[1] = shields[0].Color(20,0,0);    // Red
  colors[2] = shields[0].Color(11,5,2);    // Brown
  colors[3] = shields[0].Color(20,10,0);   // Orange
  colors[4] = shields[0].Color(20,20,0);   // Yellow
  colors[5] = shields[0].Color(0,20,0);    // Green
  colors[6] = shields[0].Color(0,20,20);   // Cyan
  colors[7] = shields[0].Color(0,0,20);    // Blue
  colors[8] = shields[0].Color(10,0,20);   // Purple
  colors[9] = shields[0].Color(20,20,20);  // White
  colors[10] = shields[0].Color(0,0,0);    // OFF
  
  //set the pin values for the Controller
  pinMode(DPAD_DOWN, INPUT);
  pinMode(DPAD_LEFT, INPUT);
  pinMode(DPAD_UP, INPUT);
  pinMode(DPAD_RIGHT, INPUT);
  pinMode(DPAD_VERIFY, INPUT);
  pinMode(COLOR_INC, INPUT);
  pinMode(COLOR_DEC, INPUT);

  digitalWrite(DPAD_DOWN, HIGH);
  digitalWrite(DPAD_LEFT, HIGH);
  digitalWrite(DPAD_UP, HIGH);
  digitalWrite(DPAD_RIGHT, HIGH);
  digitalWrite(DPAD_VERIFY, HIGH);
  digitalWrite(COLOR_INC, HIGH);
  digitalWrite(COLOR_DEC, HIGH);

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
      board[width][depth].color_id = 10;
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
      board[width][depth].color_id = 10;
    }
  }
  // Define Stick 1
  for (int i = 0; i < 5; ++i)
  {
    board[i][8].device = 0;
    board[i][8].shield = 1;
    board[i][8].color_id = 10;
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
    board[i][8].color_id = 10;
  }
  board[5][8].led = 0;
  board[6][8].led = 1;
  board[7][8].led = 3;
  board[8][8].led = 4;
  
  display_grid();

  load_puzzle("msk_009.txt", 1);//random(2,1000));

  // Clear Shields
  display_grid();
  uint32_t switchTime = millis();
  uint8_t focus[2] = {0, 0};
  uint8_t SState = 1;


  // TODO if client is disconnected first request is refresh
  while (1)
  {
    uint8_t x = focus[0];
    uint8_t y = focus[1];
    
    //turn on focus LED
    if (Serial1.available() > 0 && (char)Serial1.read() == 'U')
    {
      display_grid();
    }
    if (millis() - switchTime >= 500)
    {
      //check if LED is LOCKED
      //locked so flash white
      if(board[x][y].locked == 1)
      {
        //check if color or white
        //colored state
        if(SState == 1)
        {
          write_pixel(x,y, 9);
          SState = 0;
          
        }
        //white state
        else
        {
          write_pixel(x, y, board[x][y].color_id);
          SState = 1;
        }
      }
      //not locked switch between on and off
      else
      {
        //check if turn on or turn off
        //turn off LED
        if(SState = 1)
        {
          write_pixel(x, y, 10);
          SState = 0;
          
        }
        //turn on LED
        else
        {
          //does LED have a color assigned?
          //has a color
          if(board[x][y].color_id != 10)
          {
            write_pixel(x,y, board[x][y].color_id);
            SState = 1;
            
          }
          //doesn't have color -- flash white
          else
          {
            write_pixel(x, y, 9);
            SState = 1;
            
          }
        }
      }
      switchTime = millis();
    }
    //Check controller updates  
    //check for DPAD_UP
    if(digitalRead(DPAD_UP)==LOW /*&& focus[1]!=0*/)
    {
      //focus[1]--;
      write_pixel(x,y,board[x][y].color_id);
      SState = 1;
      Serial.println("UP");
      while(digitalRead(DPAD_UP) == LOW);
    }

    //check for DPAD_DOWN
    if(digitalRead(DPAD_DOWN)==LOW /*&& focus[1]!=8*/)
    {
      //focus[1]++;
      write_pixel(x,y,board[x][y].color_id);
      SState = 1;
      Serial.println("DOWN");
      while(digitalRead(DPAD_DOWN) == LOW);
    }

    //check for DPAD_RIGHT
    if(digitalRead(DPAD_RIGHT)==LOW /*&& focus[0]!=8*/)
    {
      //focus[0]++;
      write_pixel(x,y,board[x][y].color_id);
      SState = 1;
      Serial.println("RIGHT");
      while(digitalRead(DPAD_RIGHT) == LOW);
    }

    //check for DPAD_LEFT
    if(digitalRead(DPAD_LEFT)==LOW /*&& focus[0]!=0*/)
    {
      //focus[0]--;
      write_pixel(x,y,board[x][y].color_id);
      SState = 1;
      Serial.println("LEFT");
      while(digitalRead(DPAD_LEFT) == LOW);
    }

    //check for DPAD_VERIFY
    if(digitalRead(DPAD_VERIFY)==LOW)
    {
      Serial.println("VERIFY");
      while(digitalRead(DPAD_VERIFY) == LOW);
    }

    //check for COLOR_INC
    if(digitalRead(COLOR_INC)==LOW)
    {
      Serial.println("INCRE");
      while(digitalRead(COLOR_INC) == LOW);
    }

    //check for COLOR_DEC
    if(digitalRead(COLOR_DEC)==LOW)
    {
      Serial.println("DECRE");
      while(digitalRead(COLOR_DEC) == LOW);
    }
    //check if button pressed
    //  check if action is appropriate
    //check for requests from client
  }
}
  

void loop() {
}
