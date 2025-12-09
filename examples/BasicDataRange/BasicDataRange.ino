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

// Create a DataRange object
DataRange sensorBounds;

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

  // Get the current bounds
  float minValue = sensorBounds.min();
  float maxValue = sensorBounds.max();
  float range = sensorBounds.range();

  // Get the normalized value (0.0 to 1.0)
  float normalized = sensorBounds.normalized();

  // Print the results
  Serial.print("Raw: ");
  Serial.print(sensorValue);
  Serial.print("\t Min: ");
  Serial.print(minValue);
  Serial.print("\t Max: ");
  Serial.print(maxValue);
  Serial.print("\t Range: ");
  Serial.print(range);
  Serial.print("\t Normalized: ");
  Serial.println(normalized, 3);

  // Wait before next reading
  delay(100);
}
