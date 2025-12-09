/*
  DataEnvelope.cpp - Track a decaying envelope over streaming data
*/

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include "DataEnvelope.h"

DataEnvelope::DataEnvelope()
{
  _decay = 0.0f;
  _exponential = true;
  reset();
}

DataEnvelope::DataEnvelope(float decayAmount)
{
  _decay = decayAmount;
  _exponential = true;
  reset();
}

void DataEnvelope::setDecay(float decayAmount)
{
  _decay = decayAmount;
}

void DataEnvelope::setLinearDecay()
{
  _exponential = false;
}

void DataEnvelope::setExponentialDecay()
{
  _exponential = true;
}

void DataEnvelope::update(float value)
{
  _lastValue = value;

  if (!_initialized)
  {
    _lower = value;
    _upper = value;
    _initialized = true;
    return;
  }

  // Grow to include new extremes
  if (value > _upper)
  {
    _upper = value;
  }
  if (value < _lower)
  {
    _lower = value;
  }

  // Decay toward the current value by subtracting decay from upper and
  // adding decay to lower (linear) or by scaling distance to value (exponential).
  if (_decay > 0.0f)
  {
    if (!_exponential)
    {
      _upper -= _decay;
      _lower += _decay;
    }
    else
    {
      // exponential: shrink distance to the current value
      float factor = (1.0f - _decay);
      if (factor < 0.0f)
      {
        factor = 0.0f;
      }
      _upper = value + (_upper - value) * factor;
      _lower = value + (_lower - value) * factor;
    }

    if (_upper < value)
    {
      _upper = value;
    }
    if (_lower > value)
    {
      _lower = value;
    }
  }
}

float DataEnvelope::upper() const
{
  return _initialized ? _upper : 0.0f;
}

float DataEnvelope::lower() const
{
  return _initialized ? _lower : 0.0f;
}

float DataEnvelope::range() const
{
  return _initialized ? (_upper - _lower) : 0.0f;
}

float DataEnvelope::normalized() const
{
  return normalized(_lastValue);
}

float DataEnvelope::normalized(float value) const
{
  if (!_initialized)
  {
    return 0.0f;
  }

  float r = range();
  if (r == 0.0f)
  {
    return 0.0f;
  }

  float n = (value - _lower) / r;
  if (n < 0.0f)
    n = 0.0f;
  if (n > 1.0f)
    n = 1.0f;
  return n;
}

void DataEnvelope::reset()
{
  _upper = 0.0f;
  _lower = 0.0f;
  _lastValue = 0.0f;
  _initialized = false;
}
