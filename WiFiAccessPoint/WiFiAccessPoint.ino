#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include <EEPROM.h>

#include <FastLED.h>
#define NUM_LEDS 240
#define DATA_PIN 3

#define defaultmagicWord 0xAB45FE12

typedef struct
{
  word magicWord;
  byte rgbDataFlash[NUM_LEDS*3];
}eepromData_t;



#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);
CRGB leds[NUM_LEDS];

void updateLed();

//return  1 if there is another    RGB "R,G,B,"
//        0 if there is no another RGB "R,G,B\EOF"
//return -1 if there is no good RGB
int getRGBFromArg(String *arg, byte RGB[3]);

void handleRoot()
{
  String message = "Number of args received:";
  message += server.args() + "\n";
  for (int i = 0; i < server.args(); i++)
  {
    message += "Arg nº" + (String)i + " –> ";

    String argNameRead = server.argName(i);
    if(argNameRead.startsWith("led"))
    {
      argNameRead.remove(0,3);
      if(argNameRead.charAt(0) == 's')
      {
        message += " multiple leds";
        String ledString = server.arg(i);
        int ledCount = 0;
        int stat;
        byte ledRGB[3];
        message += "   " + String(ledString.length()) + '\n';
        Serial.print(message);
        message = String();
        do
        {
          stat = getRGBFromArg(&ledString, ledRGB);
          if( (stat != -1) && (ledCount<NUM_LEDS))
            leds[ledCount++] = CRGB(ledRGB[0],ledRGB[1],ledRGB[2]);
          message += " led number :" + String(ledCount-1) + " " + String(ledRGB[0]) + "," + String(ledRGB[1])+ "," + String(ledRGB[2]) + "   " + String(ledString.length()) + '\n';
          Serial.print(message);
          message = String();
        }
        while((stat == 1) && (ledCount<NUM_LEDS));
      }
      else
      {
        int ledNumber = argNameRead.toInt();
        message += " led number :" + String(ledNumber);
      }
      message += String("\n\r");
    }
    else
    {
      message += server.argName(i) + String(": ");
      message += server.arg(i) + String("\n\r");
    }
  }
  Serial.print(message);
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

IPAddress apIP(192, 168, 1, 1);

const byte DNS_PORT = 53;
DNSServer dnsServer;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");
  delay(1000);

  Serial.println("Loading EEPROM...");
  EEPROM.begin(sizeof(eepromData_t));
  eepromData_t* eepromdata = (eepromData_t*)EEPROM.getDataPtr();
  if(eepromdata->magicWord != defaultmagicWord)
  {
    for (int i = 0; i < sizeof(eepromData_t); i++)
    {
      EEPROM.write(i, 0);
    }
    eepromdata->magicWord = defaultmagicWord;
    EEPROM.commit();
  }

  Serial.println("Init Led...");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
  updateLed();
  
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  dnsServer.start(DNS_PORT, "*", apIP);

  //IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);
  server.on("/", handleRoot);
  /*server.onNotFound([]() {
      const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://192.168.1.1\" /></head><body><p>redirecting...</p></body>";
      server.send(200, "text/html", metaRefreshStr);
});*/

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  dnsServer.processNextRequest();
  server.handleClient();
}

int getRGBFromArg(String *arg, byte RGB[3])
{
  int separator = 0;
  String currentNum;
  Serial.print(*arg);
  Serial.print("B");
  int i;
  for(i=0;i<arg->length();i++)
  {
    if(arg->charAt(i) == ',')
    {
      Serial.print("S");
      RGB[separator] = currentNum.toInt();
      currentNum = String();
      separator++;
      if(separator == 3)
      break;
    }
    else
    {
      Serial.print("N");
      currentNum += arg->charAt(i);
    }
  }
  arg->remove(0, i);
  if(separator == 2) // Only 2 separator it it the last one
  {
    Serial.print("2\n");
    RGB[separator] = currentNum.toInt();
    currentNum = String();
    return 0;
  }
  else 
  if(separator == 3)
  {
    arg->remove(0, 1);
    Serial.print("3\n");
    return 1;
  }
  Serial.print("!\n");
  return -1;
}

void updateLed()
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
      leds[i] = CRGB(0,0,0);
    
  }
  FastLED.show(); 
}
