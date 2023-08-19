#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // Include the ArduinoJSON library

#define DHT_PIN 12 // DHT22 sensor is connected to D1 Mini pin D12
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

void loop() {
  delay(2000); // Delay for 2 seconds between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // in Celsius

  // Read the analog value from the UV sensor
  int uvValue = analogRead(UV_SENSOR_PIN);

  // Convert the analog value to voltage (assuming 3.3V reference)
  float voltage = uvValue * (3.3 / 1023.0);

  // Calculate UV intensity using the sensor's formula (you may need to adjust this based on calibration)
  float uvIntensity = voltage * 15;  // Adjust the constant based on calibration data

  // Create a JSON object
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["UserId"] = "SN001"; // Add UserId field
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["uv_intensity"] = uvIntensity;

  // Serialize the JSON object to a string
  String jsonMessage;
  serializeJson(jsonDoc, jsonMessage);

  // Publish the JSON message to the MQTT topic
  if (client.publish(mqttTopic, jsonMessage.c_str())) {
    Serial.println("Data published to MQTT");
  } else {
    Serial.println("Failed to publish data to MQTT");
  }

  delay(5000); // Delay for 5 seconds before the next reading
}
