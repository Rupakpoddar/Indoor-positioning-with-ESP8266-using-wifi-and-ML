#include "TinyMl.h"
#include <ESP8266WiFi.h>
#include <eDataStructures.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64);
#include <Fonts/FreeMonoBold9pt7b.h>

#define MAX_NETWORKS 3
#define MIN_RSSI -80

using namespace Eloquent::DataStructures;

double features[MAX_NETWORKS];
Array<String, MAX_NETWORKS> knownNetworks("");

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    knownNetworks.push("Madhupassi");
    knownNetworks.push("Fi Rupak");
    knownNetworks.push("Advait 2.4");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
    display.setFont(&FreeMonoBold9pt7b);
    display.clearDisplay();  // Clear the buffer
    display.setTextColor(WHITE);  // Set color of the text
    display.setTextWrap(true);
}

void loop() {
    scan();
    bool toPrint = true;
    for(int i = 0; i < MAX_NETWORKS; i++){
      if (features[i] == 0){
        toPrint = false;
      }
    }
    if (toPrint == true){
      int val = predict(features);
      display.clearDisplay();
      display.setCursor(0,10);
      display.print("Your\nlocation:\n");
      if (val == 0){
        display.print("Room #2");
      }
      else if (val == 1){
        display.print("Hall");
      }
      else{
        display.print("Room #1");
      }
      display.display();
    }
    delay(100);
}

void scan() {
    int numNetworks = WiFi.scanNetworks();

    resetFeatures();

    // assign RSSIs to feature vector
    for (int i = 0; i < numNetworks; i++) {
        String ssid = WiFi.SSID(i);
        uint16_t networkIndex = knownNetworks.indexOf(ssid);

        // only create feature if the current SSID is a known one
        if (!isnan(networkIndex))
            //features[networkIndex] = WiFi.RSSI(i);
            features[networkIndex] = WiFi.RSSI(i) > MIN_RSSI ? 80+WiFi.RSSI(i) : 0;
    }
}

// reset all features to 0
void resetFeatures() {
    const uint16_t numFeatures = sizeof(features) / sizeof(double);

    for (int i = 0; i < numFeatures; i++)
        features[i] = 0;
}
