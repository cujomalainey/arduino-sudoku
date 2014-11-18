// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      40

const int amplitude = 10;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
uint32_t spot = 0;
int w_red,p_red,w_green,p_green,w_blue,p_blue;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  randomSeed(6798);
  Serial.begin(9600);
  w_red = random(100);
  p_red = random();
  w_green = random(100);
  p_green = random();
  w_blue = random(100);
  p_blue = random();
}

void loop() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color((amplitude - 1)*sin((float)w_red*(float)(spot+i)/1800.0 + p_red) + amplitude,(amplitude - 1)*sin((float)w_green*(float)(spot+i)/1800.0 + p_green) + amplitude,(amplitude - 1)*sin((float)w_blue*(float)(spot+i)/1800.0 + p_blue) + amplitude)); // Moderately bright green color.
  }
  pixels.show();
  spot++;
}
