/*
  SoftDataEnvelope.cpp - Track a soft decaying envelope over streaming data
*/

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include "SoftDataEnvelope.h"

SoftDataEnvelope::SoftDataEnvelope(float decayAmount, float growthAmount)
{
  _decay = decayAmount;
  _growth = growthAmount;
  _exponentialDecay = true;
  _exponentialGrowth = true;
  reset();
}

SoftDataEnvelope::SoftDataEnvelope(float upper, float decayAmount, float growthAmount)
{
  _decay = decayAmount;
  _growth = growthAmount;
  _exponentialDecay = true;
  _exponentialGrowth = true;
  _lower = 0.0f;
  _upper = (upper < _lower) ? _lower : upper;
  _lastValue = _lower;
  _initialized = true;
}

SoftDataEnvelope::SoftDataEnvelope(float lower, float upper, float decayAmount, float growthAmount)
{
  _decay = decayAmount;
  _growth = growthAmount;
  _exponentialDecay = true;
  _exponentialGrowth = true;
  if (lower > upper)
  {
    float tmp = lower;
    lower = upper;
    upper = tmp;
  }
  _lower = lower;
  _upper = upper;
  _lastValue = lower;
  _initialized = true;
}

void SoftDataEnvelope::setDecay(float decayAmount)
{
  _decay = decayAmount;
}

void SoftDataEnvelope::setGrowth(float growthAmount)
{
  _growth = growthAmount;
}

void SoftDataEnvelope::setLinearDecay()
{
  _exponentialDecay = false;
}

void SoftDataEnvelope::setExponentialDecay()
{
  _exponentialDecay = true;
}

void SoftDataEnvelope::setLinearGrowth()
{
  _exponentialGrowth = false;
}

void SoftDataEnvelope::setExponentialGrowth()
{
  _exponentialGrowth = true;
}

void SoftDataEnvelope::update(float value)
{
  _lastValue = value;

  if (!_initialized)
  {
    _lower = value;
    _upper = value;
    _initialized = true;
    return;
  }

  // Soft growth toward new extremes: fade toward value if it's outside the range
  if (_growth > 0.0f)
  {
    if (value > _upper)
    {
      // Grow upper toward new high
      if (!_exponentialGrowth)
      {
        // Linear: step toward value
        _upper += _growth;
        if (_upper > value)
        {
          _upper = value;
        }
      }
      else
      {
        // Exponential: shrink gap between upper and value
        float factor = (1.0f - _growth);
        if (factor < 0.0f)
        {
          factor = 0.0f;
        }
        _upper = value + (_upper - value) * factor;
      }
    }
    if (value < _lower)
    {
      // Grow lower toward new low
      if (!_exponentialGrowth)
      {
        // Linear: step toward value
        _lower -= _growth;
        if (_lower < value)
        {
          _lower = value;
        }
      }
      else
      {
        // Exponential: shrink gap between lower and value
        float factor = (1.0f - _growth);
        if (factor < 0.0f)
        {
          factor = 0.0f;
        }
        _lower = value + (_lower - value) * factor;
      }
    }
  }

  // Decay toward the current value
  if (_decay > 0.0f)
  {
    if (!_exponentialDecay)
    {
      // Linear: subtract/add decay
      _upper -= _decay;
      _lower += _decay;
    }
    else
    {
      // Exponential: shrink distance to the current value
      float factor = (1.0f - _decay);
      if (factor < 0.0f)
      {
        factor = 0.0f;
      }
      _upper = value + (_upper - value) * factor;
      _lower = value + (_lower - value) * factor;
    }

    // Clamp to current value to prevent crossing
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

float SoftDataEnvelope::upper() const
{
  return _initialized ? _upper : 0.0f;
}

float SoftDataEnvelope::lower() const
{
  return _initialized ? _lower : 0.0f;
}

float SoftDataEnvelope::range() const
{
  return _initialized ? (_upper - _lower) : 0.0f;
}

float SoftDataEnvelope::normalized() const
{
  return normalized(_lastValue);
}

float SoftDataEnvelope::normalized(float value) const
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

void SoftDataEnvelope::reset()
{
  _upper = 0.0f;
  _lower = 0.0f;
  _lastValue = 0.0f;
  _initialized = false;
}
