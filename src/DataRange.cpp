/*
  Bounds.cpp - Library for dynamically finding bounds in data streams
*/

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include "DataRange.h"

// Constructor
DataRange::DataRange()
{
  reset();
}

// Constructor with initial bounds
DataRange::DataRange(float lower, float upper)
{
  setInitialBounds(lower, upper);
}

// Update bounds with a new value
void DataRange::update(float value)
{
  _lastValue = value;

  if (!_initialized)
  {
    _min = value;
    _max = value;
    _initialized = true;
  }
  else
  {
    if (value < _min)
    {
      _min = value;
    }
    if (value > _max)
    {
      _max = value;
    }
  }
}

// Set initial bounds explicitly
void DataRange::setInitialBounds(float lower, float upper)
{
  if (lower > upper)
  {
    float temp = lower;
    lower = upper;
    upper = temp;
  }

  _min = lower;
  _max = upper;
  _lastValue = lower;
  _initialized = true;
}

// Get the minimum value seen
float DataRange::min() const
{
  return _min;
}

// Get the maximum value seen
float DataRange::max() const
{
  return _max;
}

// Get the range (max - min)
float DataRange::range() const
{
  return _max - _min;
}

// Get normalized value (0.0 to 1.0) based on the most recent value
float DataRange::normalized() const
{
  return normalized(_lastValue);
}

// Get normalized value for a specific input value
float DataRange::normalized(float value) const
{
  if (!_initialized)
  {
    return 0.0;
  }

  float r = range();

  // Avoid division by zero
  if (r == 0.0)
  {
    return 0.0;
  }

  // Normalize to 0.0 - 1.0
  float normalized_val = (value - _min) / r;

  // Constrain to 0.0 - 1.0 range
  if (normalized_val < 0.0)
  {
    normalized_val = 0.0;
  }
  if (normalized_val > 1.0)
  {
    normalized_val = 1.0;
  }

  return normalized_val;
}

// Reset the bounds
void DataRange::reset()
{
  _min = 0.0;
  _max = 0.0;
  _lastValue = 0.0;
  _initialized = false;
}
