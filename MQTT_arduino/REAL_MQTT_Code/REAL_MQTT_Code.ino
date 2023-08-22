#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // Include the ArduinoJSON library

#define DHT_PIN 13 // DHT22 sensor is connected to D1 Mini pin D12
#define DHT_TYPE DHT22

#define UV_SENSOR_PIN A0 // ML8511 sensor is connected to analog pin A0

// WiFi settings
const char* ssid = "CM";
const char* password = "12345678";

// MQTT broker settings
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "1234";
const char* mqttPassword = "1234";
const char* mqttTopic = "testtopic/chur";

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect("D1MiniClient", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  delay(2000); // Delay for 2 seconds between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // in Celsius

  // Read the analog value from the UV sensor
  int uvValue = analogRead(UV_SENSOR_PIN);

  // Map the analog value to the UV index value using calibration data
  float uvIndex = mapFloat(uvValue, 0, 1023, 0, 11);

  // Create a JSON object
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["DevId"] = "M002"; // Add UserId field
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["uv_index"] = uvIndex;  // Use uvIndex instead of uvIntensity

  // Serialize the JSON object to a string
  String jsonMessage;
  serializeJson(jsonDoc, jsonMessage);

  // Publish the JSON message to the MQTT topic
  if (client.publish(mqttTopic, jsonMessage.c_str())) {
    Serial.println("Data published to MQTT");
    Serial.println(jsonMessage);
  } else {
    Serial.println("Failed to publish data to MQTT");
  }

  delay(5000); // Delay for 5 seconds before the next reading
}
