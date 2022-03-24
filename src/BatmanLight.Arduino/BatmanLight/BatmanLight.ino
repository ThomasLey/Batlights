#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

#define ESP32
#define PIN         2
#define TRIGGER_PIN 0
#define NUMPIXELS   30

int maxDelayTime = 100; // Max time to block the main loop with a delay. 
int aniDelay = 250;
int aniDelayToWait = 0; // if delay is too long, we can split wait time here (this is aniDelay modulo max wait time per cycle)
int aniLed = 0; // led we are currently at
String aniType = "single"; // here we can have different animation types. Animation types: single, kitt, invert, none
uint8_t aniColorR = 0;
uint8_t aniColorG = 0;
uint8_t aniColorB = 0;
bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm;
String hostname = wm.getWiFiHostname();
ESP8266WebServer server(80);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);

// *******************************************************************
// Web Service Endpoints
// *******************************************************************
String getStatusJson() {
  return "{\"Animation\": \"" + aniType + "\", \"Speed\": \"" + aniDelay + "\", \"Color\": \"" + String(pixels.Color(aniColorR, aniColorG, aniColorB)) + "\"}";
}

void handleRoot() {
  server.send(200, "text/plain", getStatusJson());
}

void setColorCode(String colorCode) {
  if (colorCode == "red") {
    aniColorR = 255; aniColorG = 0; aniColorB = 0;
  } else if (colorCode == "green") {
    aniColorR = 0; aniColorG = 255; aniColorB = 0;
  } else if (colorCode == "blue") {
    aniColorR = 0; aniColorG = 0; aniColorB = 255;
  } else if (colorCode == "yellow") {
    aniColorR = 255; aniColorG = 255; aniColorB = 0;
  } else if (colorCode == "cyan") {
    aniColorR = 0; aniColorG = 255; aniColorB = 255;
  } else if (colorCode == "magenta") {
    aniColorR = 255; aniColorG = 0; aniColorB = 255;
  } else if (colorCode == "black") {
    aniColorR = 0; aniColorG = 0; aniColorB = 0;
  } else if (colorCode == "white") {
    aniColorR = 255; aniColorG = 255; aniColorB = 255;
  };

  setAnimation(aniType);
}

void setSpeedCode(String speedCode) {
  if (speedCode == "turtle") {
    aniDelay = 3000;
  } else if (speedCode == "veryslow") {
    aniDelay = 1500;
  } else if (speedCode == "slow") {
    aniDelay = 500;
  } else if (speedCode == "medium") {
    aniDelay = 150;
  } else if (speedCode == "fast") {
    aniDelay = 50;
  } else if (speedCode == "veryfast") {
    aniDelay = 25;
  } else if (speedCode == "max") {
    aniDelay = 1;
  }
  
  aniDelayToWait = aniDelay;
}

void setAnimation(String animation) {
  aniType = animation;
  aniLed = 0;
  
  if (animation == "single" || animation == "kitt")
    pixels.clear(); // we start with a clean "ring"
  if (animation == "invert" || animation == "all")
    pixels.fill(pixels.Color(aniColorR, aniColorG, aniColorB)); // we start with a fill "ring"
  pixels.show();
}

void defineEndpoints() {
  server.on("/", handleRoot);

  server.on(UriBraces("/colorCode/{}"), []() {
    String color = server.pathArg(0);
    setColorCode(color);
    server.send(200, "text/plain", getStatusJson());
  });
  
  server.on(UriBraces("/animation/{}"), []() {
    String animation = server.pathArg(0);
    setAnimation(animation);
    server.send(200, "text/plain", getStatusJson());
  });
  
  server.on(UriBraces("/speedCode/{}"), []() {
    String speedCode = server.pathArg(0);
    setSpeedCode(speedCode);
    server.send(200, "text/plain", getStatusJson());
  });
}

// *******************************************************************
// Setup routine
// *******************************************************************
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  wm.setDebugOutput(false);
  if (wm.getWiFiHostname() == "") {
    wm.setHostname("batman");
  }
  hostname = wm.getWiFiHostname();
  Serial.println("\n Starting BATLIGHT as " + hostname);
  
  Serial.println("******************************************");
  Serial.println("Initialize LED strip with some animation");
  Serial.println("******************************************");
  pixels.begin();
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(i*8, 150, 0));
    pixels.show();
    delay(30);
  }
  pixels.clear();
  pixels.show();

  Serial.println("******************************************");
  Serial.println("Define Server URI Handler");
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  defineEndpoints();
  
  Serial.println("******************************************");
  Serial.println("Starting BATLIGHT networking");
  pixels.setPixelColor(1, pixels.Color(0, 0, 255));
  pixels.show();
  pinMode(TRIGGER_PIN, INPUT);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  

  pixels.setPixelColor(2, pixels.Color(0, 0, 255));
  pixels.show();
  if(wm_nonblocking) wm.setConfigPortalBlocking(false);

  Serial.println("******************************************");
  Serial.println("Configuring settings for batlights");
  pixels.setPixelColor(3, pixels.Color(0, 0, 255));
  pixels.show();
  WiFiManagerParameter custom_field1("batlightsname", "Batlights Name", "batman", 40);
  wm.addParameter(&custom_field1);

/*
  WiFiManagerParameter custom_field2; // global param (for non blocking w params)
  new (&custom_field2) WiFiManagerParameter("LedCount", "LED Count", "30", 3, "placeholder=\"30\"");
  wm.addParameter(&custom_field2);
*/
  wm.setSaveParamsCallback(saveParamCallback);

  pixels.setPixelColor(4, pixels.Color(0, 0, 255));
  pixels.show();
  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);
  wm.setClass("invert"); // set dark theme
  wm.setConfigPortalTimeout(60); // auto close configportal after n seconds
  wm.setAPClientCheck(true); // avoid timeout if client connected to softap
  bool res = wm.autoConnect("BatlightsSetup");

  if(!res) {
    Serial.println("Failed to connect or hit timeout");
    ESP.restart();
  } 

  Serial.println("******************************************");
  Serial.println("Setting mDNS");
  pixels.setPixelColor(5, pixels.Color(0, 0, 255));
  pixels.show();
  if (MDNS.begin(String(hostname))) {
    Serial.println("MDNS responder started at " + hostname);
  }
    
  Serial.println("******************************************");
  Serial.println("Starting server");
  pixels.setPixelColor(6, pixels.Color(0, 0, 255));
  pixels.show();
  server.begin();
  Serial.println("HTTP server started");

  pixels.clear();
  pixels.show();
}

void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    pixels.setPixelColor(6, pixels.Color(255, 0, 0));
    pixels.show();
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      pixels.setPixelColor(7, pixels.Color(255, 0, 0));
      pixels.show();
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        pixels.clear();
        for(int i=0; i<NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(255, 0, 64));
          pixels.show();
          delay(30);
        }
        pixels.clear();
        pixels.show();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      pixels.setPixelColor(8, pixels.Color(255, 0, 0));
      pixels.show();
      wm.setConfigPortalTimeout(180);
      
      if (!wm.startConfigPortal("BatlightsSetupAP")) {
        Serial.println("failed to connect or hit timeout");
        pixels.clear();
        for(int i=0; i<NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(255, 0, 64));
          pixels.show();
          delay(30);
        }
        pixels.clear();
        pixels.show();
        ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
      }
    }
  }
}


String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  wm.setHostname(getParam("batlightsname"));
  hostname = wm.getWiFiHostname();
  Serial.println("PARAM batlightsname = " + hostname);
}

// *******************************************************************
// Loop routine
// *******************************************************************
void loop() {
  if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
  checkButton(); // lets check for reset button

  server.handleClient();
  MDNS.update(); 

  // lets check if we need to wait another loop or can continue with next animation step
  if (aniDelayToWait > maxDelayTime) { // lets wait in small chunks
    delay(maxDelayTime);
    aniDelayToWait = aniDelayToWait - maxDelayTime;
  } else { // we can 
    delay(aniDelayToWait); // lets wait for the rest time span
    MoveNextAnimation();
    aniDelayToWait = aniDelay;
  }
}

// *******************************************************************
// Animatin Routine
// *******************************************************************
void MoveNextAnimation() {
  if (aniType == "invert") {
    pixels.setPixelColor((aniLed - 2 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR, aniColorG, aniColorB));
    pixels.setPixelColor((aniLed - 1 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/2, aniColorG/2, aniColorB/2));
    pixels.setPixelColor((aniLed + 1 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/2, aniColorG/2, aniColorB/2));
    pixels.setPixelColor(aniLed, pixels.Color(0, 0, 0));
    pixels.show();
  } else if (aniType == "kitt") {
    pixels.setPixelColor((aniLed - 3 + NUMPIXELS) % NUMPIXELS, pixels.Color(0, 0, 0));
    pixels.setPixelColor((aniLed - 2 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/16, aniColorG/16, aniColorB/16));
    pixels.setPixelColor((aniLed + 2 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/16, aniColorG/16, aniColorB/16));
    pixels.setPixelColor((aniLed - 1 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/4, aniColorG/4, aniColorB/4));
    pixels.setPixelColor((aniLed + 1 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/4, aniColorG/4, aniColorB/4));
    pixels.setPixelColor(aniLed, pixels.Color(aniColorR, aniColorG, aniColorB));
    pixels.show();
  }  else if (aniType == "single") {
    pixels.setPixelColor((aniLed - 2 + NUMPIXELS) % NUMPIXELS, pixels.Color(0, 0, 0));
    pixels.setPixelColor((aniLed - 1 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/16, aniColorG/16, aniColorB/16));
    pixels.setPixelColor((aniLed + 1 + NUMPIXELS) % NUMPIXELS, pixels.Color(aniColorR/16, aniColorG/16, aniColorB/16));
    pixels.setPixelColor(aniLed, pixels.Color(aniColorR, aniColorG, aniColorB));
    pixels.show();
  } else { // fallback "all"
    delay(maxDelayTime); 
    return;  
  }
  
  aniLed = (aniLed + 1) % NUMPIXELS;
}
