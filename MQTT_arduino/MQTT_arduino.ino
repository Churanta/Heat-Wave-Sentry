#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN 12
#define DHTTYPE DHT22
#define ML8511PIN 13

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  float uvIntensity = analogRead(ML8511PIN);
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("UV Intensity: ");
  Serial.println(uvIntensity);
  
  delay(2000);
}
