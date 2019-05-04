#include "lfo.h"
#include <math.h>
const float PI = 3.1415927;

CoreWave::CoreWave(int length) : Wave() {
    this->length = length;
    this->pos = 0;
    this->value = 0.0;
}

SineWave::SineWave(int length) : CoreWave(length) {
        this->divs = 2 * PI / length;
}
    
float SineWave::next() {
    value = sin(pos * divs);
    pos = ++pos % length;
    return value;
}

SawWave::SawWave(int length) : CoreWave(length) {
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

TriWave::TriWave(int length) : CoreWave(length) {
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

SquareWave::SquareWave(int length) : CoreWave(length) {
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
