/*
  DataRange.h - Library for dynamically finding bounds in data streams

  This library allows you to track the minimum and maximum values
  in a stream of data and provides normalized values between 0.0 and 1.0.
*/

#ifndef DataRange_h
#define DataRange_h

#ifdef ARDUINO
#include "Arduino.h"
#endif

class DataRange
{
public:
  // Constructor
  DataRange();

  // Constructor with starting upper bound (lower fixed at 0)
  DataRange(float upper);

  // Constructor with initial bounds
  DataRange(float lower, float upper);

  // Enable/disable floating lower bound (default: fixed at 0)
  void setFloatingLower(bool enable);

  // Update bounds with a new value
  void update(float value);

  // Get the minimum value seen
  float min() const;

  // Get the maximum value seen
  float max() const;

  // Get the range (max - min)
  float range() const;

  // Get normalized value (0.0 to 1.0) based on the most recent value
  float normalized() const;

  // Get normalized value for a specific input value
  float normalized(float value) const;

  // Set initial bounds explicitly
  void setInitialBounds(float lower, float upper);

  // Reset the bounds
  void reset();

private:
  float _min;
  float _max;
  float _lastValue;
  bool _initialized;
  bool _floatLower;
};

#endif
