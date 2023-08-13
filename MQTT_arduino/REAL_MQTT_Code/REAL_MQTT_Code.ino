#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 12
#define DHTTYPE DHT22

// WiFi network credentials
const char* ssid = "Churanta";
const char* password = "Kanasona12!";

// MQTT broker information
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_user = "1234";
const char* mqtt_password = "1234";

// MQTT topic to publish data to
const char* mqtt_topic = "testtopic/chur";

// DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT callback function prototype
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup() {
  // Initialize serial port
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Connect to MQTT broker
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqtt_callback);
// Inside the setup function where you connect to the MQTT broker
while (!client.connected()) {
    Serial.print("Attempting to connect to MQTT broker...");
    // Concatenate username and password
    String clientId = String(mqtt_user) + "-" + String(mqtt_password);
    if (client.connect(clientId.c_str())) {
        Serial.println("Connected");
    } else {
        Serial.println("Failed, retrying in 5 seconds");
        delay(5000);
    }
}

}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Publish data to MQTT broker
  String payload = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
  client.publish(mqtt_topic, payload.c_str());

  // Wait 2 seconds before publishing again
  delay(2000);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // Do nothing
}
