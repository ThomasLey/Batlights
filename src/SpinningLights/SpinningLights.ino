#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include <uri/UriBraces.h>

#include "Secrets.s.h"

#define ESP32
#define DATAPIN      2
#define NUMPIXELS    150
#define DEF_HOSTNAME "RUNLI"
String defaultHostname = DEF_HOSTNAME;

// config settings which can be set through api directly
uint32_t backgroundColor = 8388704; // some violett
uint32_t foregroundColor = 16714770; // CGI red
int direction = 1; // which means single step forward
int pps = 3; // pixels per segment
int nos = 2; // number of segments

// config which is used for animation
int maxDelayTime = 500; // Max time to block the main loop with a delay. 
int aniDelay = 250; // delay which was set
int aniDelayToWait = 0; // if delay is too long, we can split wait time here (this is aniDelay minus maxDelayTime per cycle)
int aniLed = 0; // led we are currently at
int aniCount = 0; // count number of animations for logging
String aniType = "void"; // values: rotate, highlight, void

// create objects of interest 
WiFiClient client;
ESP8266WebServer server(80);
Adafruit_NeoPixel pixels(NUMPIXELS, DATAPIN, NEO_GRB + NEO_KHZ400);

// *******************************************************************
// Web Service Endpoints
// *******************************************************************
String getStatusJson() {
  String result = "{\r\n";
  for(int i=0; i<NUMPIXELS; i++) {
    uint32_t pColor = pixels.getPixelColor(i);
    result += "  \""+String(i)+"\": "+pColor+",\r\n";
  }
  result += "  \"mdns\": \""+defaultHostname+"\",\r\n";
  result += "  \"speed\": "+String(aniDelay)+",\r\n";
  result += "  \"background\": "+String(backgroundColor)+",\r\n";
  result += "  \"foreground\": "+String(foregroundColor)+",\r\n";
  result += "  \"direction\": "+String(direction)+",\r\n";
  result += "  \"pps\": "+String(pps)+",\r\n";
  result += "  \"nos\": "+String(nos)+",\r\n";
  result += "  \"animation\": \""+aniType+"\"\r\n";
  result += "}";

  return result;
}

void handleRoot() {
  server.send(200, "text/plain", getStatusJson());
}

void defineEndpoints() {
  server.on("/", handleRoot);

  server.on(UriBraces("/setBackground/{}"), []() {
    backgroundColor = server.pathArg(0).toInt();
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/setForeground/{}"), []() {
    foregroundColor = server.pathArg(0).toInt();
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/setDirection/{}"), []() {
    direction = server.pathArg(0).toInt();
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/setPps/{}"), []() {
    pps = server.pathArg(0).toInt();
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/setNos/{}"), []() {
    nos = server.pathArg(0).toInt();
    server.send(200, "text/plain", getStatusJson());
  });


  server.on(UriBraces("/speed/{}"), []() {
    String speedStr = server.pathArg(0);
    float rps = speedStr.toFloat();
    aniDelay = (1000/NUMPIXELS) / rps;
    aniDelayToWait = aniDelay;
    aniType = "rotate";
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/highlight/"), []() {
    Serial.println("setting highlight");
    aniType = "highlight";
    aniDelay = 1000;
    aniDelayToWait = aniDelay;
    // create ui setup
    pixels.fill(backgroundColor);
    pixels.setPixelColor(0, foregroundColor);
    pixels.show();
    server.send(200, "text/plain", getStatusJson());
  });

  server.on("/off", []() {
    Serial.println("setting off");
    pixels.clear();
    pixels.show();
    aniType = "off";
    aniDelay = 1000;
    aniDelayToWait = aniDelay;
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/on/{}"), []() {
    Serial.println("setting on");
    String colorStr = server.pathArg(0);
    pixels.fill(colorStr.toInt());
    pixels.show();
    aniType = "off";
    aniDelay = 1000;
    aniDelayToWait = aniDelay;
    server.send(200, "text/plain", getStatusJson());
  });
}

void setup() {
  Serial.begin(115200);

  Serial.println("Initialize LED strip with some animation");
  pixels.begin();
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(i*8, 150, 0));
    pixels.show();
    delay(10);
  }
  delay(2000);
  pixels.clear();
  pixels.show();
  
  Serial.println("Initialize WLAN");
  pixels.setPixelColor(0, pixels.Color(0, 150, 0)); pixels.show();
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected. IP address: "); Serial.println(WiFi.localIP());

  Serial.println("Define Server URI Handler");
  pixels.setPixelColor(1, pixels.Color(0, 150, 0)); pixels.show();
  defineEndpoints();

  Serial.println("Starting server");
  pixels.setPixelColor(2, pixels.Color(0, 150, 0)); pixels.show();
  server.begin();

  Serial.println("Setting mDNS");
  pixels.setPixelColor(3, pixels.Color(0, 150, 0)); pixels.show();
  if (MDNS.begin(defaultHostname)) {
    Serial.println("MDNS responder started at " + defaultHostname);
  }

  delay(2000);
  pixels.clear();
  pixels.show();
}

void loop() {
  server.handleClient();
  MDNS.update();

  // lets check if we need to wait another loop or can continue with next animation step
  if (aniDelayToWait > maxDelayTime) { // lets wait in small chunks
    delay(maxDelayTime);
    aniDelayToWait = aniDelayToWait - maxDelayTime;
  } else { // we can do the next animation
    delay(aniDelayToWait); // lets wait for the rest time span
    MoveNextAnimation();
    aniDelayToWait = aniDelay; // lets set next animation delay
  }
}

void MoveNextAnimation() {
  Serial.println("Next animation + " + String(aniCount++));
  if (aniType == "rotate") {
    int segmentDelta = NUMPIXELS / nos;
    pixels.fill(backgroundColor);
    for (int i = 0; i < nos; i++) {
      int offset = ((i * segmentDelta)+aniLed) % NUMPIXELS;
      for (int j = 0; j < pps; j++) {
        int pxl = (j + offset) % NUMPIXELS;
        pixels.setPixelColor(pxl, foregroundColor);
      }
    }
    pixels.show();
    aniLed = (aniLed + direction + NUMPIXELS) % NUMPIXELS;
  } else if (aniType == "highlight") {
    delay(maxDelayTime);
  } else { // default "viod"
    delay(maxDelayTime);
  }
}
