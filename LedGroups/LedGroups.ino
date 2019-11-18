#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS_MAX 240
#define DATA_PIN 3
typedef enum
{
  LED_GROUP_ARG,
  LED_GROUP_G,
  LED_GROUP_AVG,
  LED_GROUP_AVD,
  LED_GROUP_D,
  LED_GROUP_ARD,
  LED_GROUPS_COUNT
} ledGroupsEnum_t;

int ledGroupCount[LED_GROUPS_COUNT] = {10,25,15,16,26,10};
int ledGroupCountIndex[LED_GROUPS_COUNT];
CRGB ledGroupColor[LED_GROUPS_COUNT];
// Define the array of leds
CRGB leds[NUM_LEDS_MAX];
int totalLeds = 0;

float currentCount();

void setup()
{ 
	Serial.begin(115200);
	Serial.println("resetting");

  ledGroupColor[LED_GROUP_ARG] = CRGB(255,0,0); //Arrière rouge
  ledGroupColor[LED_GROUP_G] = CRGB(255,0,0); //Gauche Orange
  ledGroupColor[LED_GROUP_AVG] = CRGB(255,255,255); //Avant Blanc
  ledGroupColor[LED_GROUP_AVD] = ledGroupColor[LED_GROUP_AVG];
  ledGroupColor[LED_GROUP_D] = ledGroupColor[LED_GROUP_G];
  ledGroupColor[LED_GROUP_ARD] = ledGroupColor[LED_GROUP_ARG];


  
  int lastGroupCountIndex = 0;
  for(int i=0;i<LED_GROUPS_COUNT;i++)
  {
    ledGroupCountIndex[i] = lastGroupCountIndex;
    lastGroupCountIndex += ledGroupCount[i];
    totalLeds+=ledGroupCount[i];
  }
  if(totalLeds > NUM_LEDS_MAX)
    totalLeds = NUM_LEDS_MAX-1;
  
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds, totalLeds);
	LEDS.setBrightness(84);

   for(int i=0;i<totalLeds;i++)
  {
    for(int igroup=0;igroup<LED_GROUPS_COUNT;igroup++)
    {
      if(i>ledGroupCountIndex[igroup])
      {
        leds[i] = ledGroupColor[igroup];
      }
      else
      {
        break;
      }
    }
    /*
    if(i < 2)
      leds[i] = CRGB(255,0,0);
    else if(i<4)
      leds[i] = CRGB(0,255,0);
    else if(i<6)
      leds[i] = CRGB(0,0,255);
    else
      leds[i] = CRGB(255,255,0);
    */
  }
  FastLED.show(); 
}

#define currentPerColor 18.5

float currentCount()
{
  float current;


  return current;
}


void loop()
{

}
