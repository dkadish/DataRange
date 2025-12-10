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
#include <DataEnvelope.h>
#include <SoftDataEnvelope.h>

// Track raw bounds for calibration
DataRange sensorBounds;
// Quick envelope (fast attack, decays toward recent values)
DataEnvelope fastEnv(0.1f); // 10% exponential decay per update
// Soft envelope (slower attack toward peaks + decay)
SoftDataEnvelope softEnv(0.05f, 0.15f); // 5% decay, 15% growth (exponential)

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
  fastEnv.update(sensorValue);
  softEnv.update(sensorValue);

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
    // Normalized values (raw range, fast envelope, soft envelope)
    float rangeNorm = sensorBounds.normalized();
    float fastNorm = fastEnv.normalized(sensorValue);
    float softNorm = softEnv.normalized(sensorValue);

    // Convert to PWM range (0-255)
    int brightness = (int)(softNorm * 255); // drive LED with softened envelope

    // Set LED brightness
    analogWrite(ledPin, brightness);

    // Print values
    Serial.print("Sensor:");
    Serial.print(sensorValue);
    Serial.print(",RangeNorm:");
    Serial.print(rangeNorm, 3);
    Serial.print(",FastEnvNorm:");
    Serial.print(fastNorm, 3);
    Serial.print(",SoftEnvNorm:");
    Serial.print(softNorm, 3);
    Serial.print(",LED Brightness:");
    Serial.println(brightness);
  }

  delay(100);
}
