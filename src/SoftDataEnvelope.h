/*
  SoftDataEnvelope.h - Track a soft decaying envelope over streaming data

  Upper and lower bounds grow softly toward new extremes and decay toward
  recent values using configurable per-update growth and decay amounts.
  Both growth and decay use linear or exponential strategies.
*/

#ifndef SoftDataEnvelope_h
#define SoftDataEnvelope_h

#ifdef ARDUINO
#include "Arduino.h"
#endif

class SoftDataEnvelope
{
public:
  explicit SoftDataEnvelope(float decayAmount, float growthAmount);
  SoftDataEnvelope(float upper, float decayAmount, float growthAmount);
  SoftDataEnvelope(float lower, float upper, float decayAmount, float growthAmount);

  void setDecay(float decayAmount);
  void setGrowth(float growthAmount);

  // Switch between exponential (default) and linear decay modes
  void setLinearDecay();
  void setExponentialDecay();

  // Switch between exponential (default) and linear growth modes
  void setLinearGrowth();
  void setExponentialGrowth();

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
  float _growth;
  bool _exponentialDecay;
  bool _exponentialGrowth;
  bool _initialized;
};

#endif
