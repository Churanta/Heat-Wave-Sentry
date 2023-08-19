#include <Arduino.h>

// Define the analog pin for the UV sensor
#define UV_SENSOR_PIN A0

void setup() {
  // Initialize serial communication at 9600 bps
  Serial.begin(9600);

  // Print a start message
  Serial.println("UV Sensor Test");

  // You can add any other setup code here
}

void loop() {
  // Read the analog value from the UV sensor
  int uvValue = analogRead(UV_SENSOR_PIN);

  // Convert the analog value to voltage (assuming 3.3V reference)
  float voltage = uvValue * (3.3 / 1023.0);

  // Calculate UV intensity using the sensor's formula (you may need to adjust this based on your calibration)
  float uvIntensity = voltage * 15;  // Adjust the constant based on calibration data

  // Print the UV sensor data
  Serial.print("Analog Value: ");
  Serial.print(uvValue);
  Serial.print(" - Voltage: ");
  Serial.print(voltage);
  Serial.print("V - UV Intensity: ");
  Serial.println(uvIntensity);

  // You can add a delay here if needed
  delay(1000);
}