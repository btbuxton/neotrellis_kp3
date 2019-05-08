#include "lfo.h"
#include <math.h>
const float PI = 3.1415927;

CoreWave::CoreWave() : Wave() {
  setLength(1);
}

void CoreWave::setLength(int length) {
  this->length = length;
  this->pos = 0;
  this->value = 0.0;
}

SineWave::SineWave() : CoreWave() {
}

void SineWave::setLength(int length) {
  CoreWave::setLength(length);
  this->divs = 2 * PI / length;
}

    
float SineWave::next() {
    value = sin(pos * divs);
    pos = ++pos % length;
    return value;
}

SawWave::SawWave() : CoreWave() {
}

void SawWave::setLength(int length) {
  CoreWave::setLength(length);
  this->slope = 2 / (float)length;
}

float SawWave::next() {
    value = slope * pos;
    if (pos > length / 2) {
        value = slope * pos - 2;
    }
    pos = ++pos % length;
    return value;
}

TriWave::TriWave() : CoreWave() {
}

void TriWave::setLength(int length) {
  CoreWave::setLength(length);
  this->slope = 4 / (float)length;
}

float TriWave::next() {
    value = slope * pos;
    if (pos > 3 * length / 4) {
        value = value - 4;
    } else if (pos > length / 4) {
        value = 2 - value;
    }
    pos = ++pos % length;
    return value;
}

SquareWave::SquareWave() : CoreWave() {
}

float SquareWave::next() {
    value = 1;
    if (pos > length / 2) {
        value = -1;
    }
    pos = ++pos % length;
    return value;
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
