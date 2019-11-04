#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

//return  1 if there is another    RGB "R,G,B,"
//        0 if there is no another RGB "R,G,B\EOF"
//return -1 if there is no good RGB

int getRGBFromArg(String *arg, int RGB[3]);

void handleRoot()
{
  String message = "Number of args received:";
  message += server.args();
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

void setup() {
  delay(1000);
  Serial.begin(115200);
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

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

int getRGBFromArg(String *arg, int RGB[3])
{
  int separator = 3;
  for(int i=0;i<
  if(arg.charAt(0) == ','

  
}
