/*
  Sensor Normalization Example

  This example shows how to use the DataRange library to normalize
  sensor readings for use in applications like LED brightness control
  or motor speed control.

  The circuit:
  - Potentiometer connected to analog pin A0
  - LED connected to digital pin 9 (PWM capable)

  Created December 9, 2025
*/

#include <DataRange.h>

// Create a DataRange object
DataRange sensorBounds;

const int sensorPin = A0;
const int ledPin = 9;

// Calibration time in milliseconds
const unsigned long calibrationTime = 5000;
unsigned long startTime;
bool calibrating = true;

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);

  // Set LED pin as output
  pinMode(ledPin, OUTPUT);

  Serial.println("Sensor Normalization Example");
  Serial.println("Move the potentiometer through its full range for 5 seconds...");
  Serial.println();

  startTime = millis();
}

void loop()
{
  // Read the sensor
  int sensorValue = analogRead(sensorPin);

  // Update bounds
  sensorBounds.update(sensorValue);

  // Check if we're still calibrating
  if (calibrating)
  {
    if (millis() - startTime < calibrationTime)
    {
      Serial.print("Calibrating... Min: ");
      Serial.print(sensorBounds.min());
      Serial.print(" Max: ");
      Serial.println(sensorBounds.max());
    }
    else
    {
      calibrating = false;
      Serial.println("Calibration complete!");
      Serial.println();
    }
  }
  else
  {
    // Get normalized value (0.0 to 1.0)
    float normalized = sensorBounds.normalized();

    // Convert to PWM range (0-255)
    int brightness = (int)(normalized * 255);

    // Set LED brightness
    analogWrite(ledPin, brightness);

    // Print values
    Serial.print("Sensor: ");
    Serial.print(sensorValue);
    Serial.print("\t Normalized: ");
    Serial.print(normalized, 3);
    Serial.print("\t LED Brightness: ");
    Serial.println(brightness);
  }

  delay(100);
}
