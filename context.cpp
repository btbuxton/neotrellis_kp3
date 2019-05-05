#include "context.h"
#include "layout.h"
#include "constants.h"
#include <Adafruit_ADXL343.h>

uint32_t micros_per_pulse(uint16_t tempo) {
  return (uint32_t)((SEC_PER_MIN * MS_PER_SEC * MICROS_PER_MS) / (float)(PPQN * tempo));
}

Context::Context(Adafruit_NeoTrellisM4* trellis, Adafruit_ADXL343* accel, Layout* initial) {
  _trellis = trellis;
  _accel = accel;
  _layout = initial;
  setTempo(120);
}

Adafruit_NeoTrellisM4* Context::trellis() {
  return _trellis;
}

Adafruit_ADXL343* Context::accel() {
  return _accel;
}

Layout* Context::layout() {
  return _layout;
}

void Context::setLayout(Layout* newLayout) {
  _layout = newLayout;
}

uint16_t Context::tempo() {
  return _tempo;
}

void Context::setTempo(uint16_t newTempo) {
  _tempo = newTempo;
  _clockDelay = micros_per_pulse(_tempo);
}

uint32_t Context::clockDelay() {
  return _clockDelay;
}

void Context::refresh() {
  _layout->refresh(this);
}

void Context::update() {
  _layout->update(this);
}

void Context::changeToNextLayout() {
  _layout = _layout->next();
  refresh();
}
