#include <ESP8266WiFi.h>

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}

void loop() {
  int numNetworks = WiFi.scanNetworks();

  for (int i = 0; i < numNetworks; i++) {
      Serial.println(WiFi.SSID(i));

  delay(3000);
  }
}
