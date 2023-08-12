#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Replace with your Wi-Fi credentials
const char* ssid = "Churanta";
const char* password = "Kanasona12!";

// MQTT Broker information
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "1234";
const char* mqttPassword = "1234";

WiFiClient espClient;
PubSubClient client(espClient);

// DHT22 Sensor
#define DHTPIN 12         // Pin where the DHT22 is connected
#define DHTTYPE DHT22     // Type of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 2000; // Interval to send data (2 seconds)

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT Broker
  client.setServer(mqttServer, mqttPort);

  if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT Broker");

    // Subscribe to the MQTT topic
    client.subscribe("testtopic/chur");
  } else {
    Serial.println("Failed to connect to MQTT Broker");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received MQTT message on topic: ");
  Serial.println(topic);

  // Convert payload to a string
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message received: ");
  Serial.println(message);
}

void loop() {
  unsigned long currentMillis = millis();

  if (!client.connected()) {
    Serial.println("Reconnecting to MQTT Broker...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("Reconnected to MQTT Broker");
      client.subscribe("testtopic/chur");
    } else {
      Serial.println("Failed to reconnect to MQTT Broker");
    }
  }

  client.loop();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read DHT22 sensor data
    dht.begin(); // Start the sensor
    delay(200);  // Wait for sensor to stabilize
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      // Publish sensor data to MQTT
      String payload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
      client.publish("testtopic/chur", payload.c_str());
      Serial.println("Sensor data published to MQTT");
    } else {
      Serial.println("Failed to read sensor data");
    }
  }
}
