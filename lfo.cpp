#include "lfo.h"
#include <math.h>
const float PI = 3.1415927;

CoreWave::CoreWave() : Wave() {
  setLength(1);
}

void CoreWave::setLength(int length) {
  this->length = length;
  this->pos = 0;
  this->_value = 0.0;
}

void CoreWave::reset() {
  this->pos = 0;
}

float CoreWave::value() {
  return _value;
}

SineWave::SineWave() : CoreWave() {
}

void SineWave::setLength(int length) {
  CoreWave::setLength(length);
  this->divs = 2 * PI / length;
}

    
float SineWave::next() {
    _value = sin(pos * divs);
    pos = ++pos % length;
    return _value;
}

SawWave::SawWave() : CoreWave() {
}

void SawWave::setLength(int length) {
  CoreWave::setLength(length);
  this->slope = 2 / (float)length;
}

float SawWave::next() {
    _value = slope * pos;
    if (pos > length / 2) {
        _value = slope * pos - 2;
    }
    pos = ++pos % length;
    return _value;
}

TriWave::TriWave() : CoreWave() {
}

void TriWave::setLength(int length) {
  CoreWave::setLength(length);
  this->slope = 4 / (float)length;
}

float TriWave::next() {
    _value = slope * pos;
    if (pos > 3 * length / 4) {
        _value = _value - 4;
    } else if (pos > length / 4) {
        _value = 2 - _value;
    }
    pos = ++pos % length;
    return _value;
}

SquareWave::SquareWave() : CoreWave() {
}

float SquareWave::next() {
    _value = 1;
    if (pos > length / 2) {
        _value = -1;
    }
    pos = ++pos % length;
    return _value;
}

NegWave::NegWave(Wave* wave) : Wave() {
    this->wave = wave;
}

float NegWave::next() {
    return -wave->next();
}

void NegWave::setLength(int length) {
  wave->setLength(length);
}

void NegWave::reset() {
  wave->reset();
}

float NegWave::value() {
  return wave->value();
}

LFO::LFO() {
  _length = 24;
  _wave = _all[0];
}
Wave** LFO::all() {
  return _all;
}

float LFO::next() {
  return _wave->next();  
}

float LFO::value() {
  return _wave->value();
}

void LFO::reset() {
  _wave->setLength(_length);
  _wave->reset();
}

void LFO::setLength(int length) {
  _length = length;
  reset();
}

void LFO::setWave(Wave* wave) {
  _wave = wave;
  reset();
}
