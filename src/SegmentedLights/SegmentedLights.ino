#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include <uri/UriBraces.h>

#include "Secrets.s.h"

#define ESP32
#define DATAPIN      2
#define NUMPIXELS    150
#define PPS          10
#define DEF_HOSTNAME "SEGLI"

uint32_t lastSetColor = 0; // which is basically off
uint32_t backgroundColor = 0;
int maxDelayTime = 500; // Max time to block the main loop with a delay. 
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
  result += "  \"mdns\": \""+defaultHostname+"\",\r\n";
  result += "  \"backgroundColor\": \""+String(backgroundColor)+"\"\r\n";
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

  /* configuration endpoints */
  server.on(UriBraces("/setBackground/{}"), []() {
    backgroundColor = server.pathArg(0).toInt();
    server.send(200, "text/plain", getStatusJson());
  });

  /* business endpoints */
  server.on(UriBraces("/segment/{}/color/{}"), []() {
    String segmentStr = server.pathArg(0);
    String colorStr = server.pathArg(1);
    setSegment(segmentStr.toInt(), colorStr.toInt());    
    server.send(200, "text/plain", getStatusJson());
  });

  server.on(UriBraces("/xsegment/{}/color/{}"), []() {
    String segmentStr = server.pathArg(0);
    String colorStr = server.pathArg(1);
    pixels.fill(backgroundColor);
    setSegment(segmentStr.toInt(), colorStr.toInt());    
    server.send(200, "text/plain", getStatusJson());
  });

  server.on("/clear", []() {
    pixels.fill(backgroundColor);
    pixels.show();
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
    delay(10);
  }
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
  delay(maxDelayTime);
}
