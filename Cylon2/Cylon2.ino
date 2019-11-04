#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 8 
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	Serial.begin(115200);
	Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	LEDS.setBrightness(84);
}


void loop()
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    if(i < 2)
      leds[i] = CRGB(255,0,0);
    else if(i<4)
      leds[i] = CRGB(0,255,0);
    else if(i<6)
      leds[i] = CRGB(0,0,255);
    else
      leds[i] = CRGB(255,255,0);
    
  }
  FastLED.show(); 
}
