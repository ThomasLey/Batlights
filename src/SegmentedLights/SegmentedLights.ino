#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include <uri/UriBraces.h>

#include "Secrets.s.h"

#define ESP32
#define DATAPIN      2
#define NUMPIXELS    300
#define PPS          5
#define DEF_HOSTNAME "SEGLI"

uint32_t lastSetColor = 0; // which is basically off
int maxDelayTime = 100; // Max time to block the main loop with a delay. 
int aniDelay = 250;
int aniDelayToWait = 0; // if delay is too long, we can split wait time here (this is aniDelay modulo max wait time per cycle)
int aniLed = 0; // led we are currently at
String defaultHostname = DEF_HOSTNAME;

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
  result += "  \"mdns\": \""+defaultHostname+"\"\r\n";
  result += "}";

  return result;
}

void handleRoot() {
  server.send(200, "text/plain", getStatusJson());
}

void setSegment(uint32_t segment, uint32_t color) {
  for(int i=(PPS*segment); i<(PPS*(segment+1)); i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}

void defineEndpoints() {
  server.on("/", handleRoot);

  server.on(UriBraces("/segment/{}/color/{}"), []() {
    String segmentStr = server.pathArg(0);
    String colorStr = server.pathArg(1);
    setSegment(segmentStr.toInt(), colorStr.toInt());    
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/xsegment/{}/color/{}"), []() {
    String segmentStr = server.pathArg(0);
    String colorStr = server.pathArg(1);
    pixels.clear();
    setSegment(segmentStr.toInt(), colorStr.toInt());    
    server.send(200, "text/plain", getStatusJson());
  });

  server.on("/off", []() {
    pixels.clear();
    pixels.show();
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/on/{}"), []() {
    String colorStr = server.pathArg(0);
    pixels.fill(colorStr.toInt());
    pixels.show();
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
    delay(30);
  }
  pixels.clear();
  pixels.show();
  
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected. IP address: "); Serial.println(WiFi.localIP());

  Serial.println("Define Server URI Handler");
  defineEndpoints();

  Serial.println("Starting server");
  server.begin();

  Serial.println("Setting mDNS");
  if (MDNS.begin(defaultHostname)) {
    Serial.println("MDNS responder started at " + defaultHostname);
  }
}

void loop() {
  server.handleClient();
  MDNS.update(); 
}
