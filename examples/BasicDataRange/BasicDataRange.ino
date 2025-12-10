/*
  Basic DataRange Example

  This example demonstrates how to use the DataRange library to track
  the minimum and maximum values of a data stream and obtain
  normalized values between 0.0 and 1.0.

  The circuit:
  - Potentiometer connected to analog pin A0

  Created December 9, 2025
*/

#include <DataRange.h>
#include <DataEnvelope.h>
#include <SoftDataEnvelope.h>

// Track raw bounds
DataRange sensorBounds;
// Fast envelope that jumps to new peaks and decays toward recent values
DataEnvelope env(0.15f); // 15% exponential decay per update
// Soft envelope that eases toward new peaks and decays
SoftDataEnvelope softEnv(0.10f, 0.20f); // 10% decay, 20% growth (exponential)

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);

  Serial.println("DataRange Library - Basic Example");
  Serial.println("Reading analog input and tracking bounds...");
  Serial.println();
}

void loop()
{
  // Read analog sensor value (0-1023)
  int sensorValue = analogRead(A0);

  // Update the bounds with the new value
  sensorBounds.update(sensorValue);
  env.update(sensorValue);
  softEnv.update(sensorValue);

  // Get the current bounds
  float minValue = sensorBounds.min();
  float maxValue = sensorBounds.max();
  float range = sensorBounds.range();

  // Get normalized values
  float normalized = sensorBounds.normalized();
  float envNorm = env.normalized(sensorValue);
  float softEnvNorm = softEnv.normalized(sensorValue);

  // Print the results
  Serial.print("Raw: ");
  Serial.print(sensorValue);
  Serial.print("\t Min: ");
  Serial.print(minValue);
  Serial.print("\t Max: ");
  Serial.print(maxValue);
  Serial.print("\t Range: ");
  Serial.print(range);
  Serial.print("\t RangeNorm: ");
  Serial.print(normalized, 3);
  Serial.print("\t EnvNorm: ");
  Serial.print(envNorm, 3);
  Serial.print("\t SoftEnvNorm: ");
  Serial.println(softEnvNorm, 3);

  // Wait before next reading
  delay(100);
}
