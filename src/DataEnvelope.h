/*
  DataEnvelope.h - Track a decaying envelope over streaming data

  Upper and lower bounds grow to include new extremes and decay toward
  recent values using a configurable per-update decay amount.
*/

#ifndef DataEnvelope_h
#define DataEnvelope_h

#ifdef ARDUINO
#include "Arduino.h"
#endif

class DataEnvelope
{
public:
  DataEnvelope();
  explicit DataEnvelope(float decayAmount);

  void setDecay(float decayAmount);
  void update(float value);

  float upper() const;
  float lower() const;
  float range() const;

  float normalized() const;
  float normalized(float value) const;

  void reset();

private:
  float _upper;
  float _lower;
  float _lastValue;
  float _decay;
  bool _initialized;
};

#endif
