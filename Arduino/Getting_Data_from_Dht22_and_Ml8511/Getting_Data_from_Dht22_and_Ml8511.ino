#include <DHT.h>

#define DHT_PIN 12 // DHT22 sensor is connected to D1 Mini pin D12
#define DHT_TYPE DHT22

#define UV_SENSOR_PIN A0 // ML8511 sensor is connected to analog pin A0

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
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

  // Print the sensor data in JSON format
  Serial.print("{");
  Serial.print("\"temperature\": ");
  Serial.print(temperature);
  Serial.print(", ");
  Serial.print("\"humidity\": ");
  Serial.print(humidity);
  Serial.print(", ");
  Serial.print("\"uv_intensity\": ");
  Serial.print(uvIntensity);
  Serial.println("}");

  delay(5000); // Delay for 5 seconds before the next reading
}
