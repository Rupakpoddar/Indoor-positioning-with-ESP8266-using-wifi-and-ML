#include <ESP8266WiFi.h>
#include <eDataStructures.h>

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
    /*
    knownNetworks.push("IndoorPositioning1");
    knownNetworks.push("Fi Rupak");
    knownNetworks.push("Sharma 130");
    knownNetworks.push("Surya");
    knownNetworks.push("Madhupassi");
    knownNetworks.push("sohal146");
    knownNetworks.push("kailash");
    knownNetworks.push("Advait 2.4");
    knownNetworks.push("Aswini 2g");
    */
}

void loop() {
    scan();
    printFeatures();
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

void printFeatures() {
    const uint16_t numFeatures = sizeof(features) / sizeof(double);
    
    for (int i = 0; i < numFeatures; i++) {
        Serial.print(features[i]);
        Serial.print(i == numFeatures - 1 ? "\t3\n" : "\t");
    }
}
