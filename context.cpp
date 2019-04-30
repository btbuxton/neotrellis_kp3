#include "context.h"
#include "layout.h"

#define PPQN 24
#define MS_PER_SEC 1000
#define MICROS_PER_MS 1000
#define SEC_PER_MIN 60

uint32_t micros_per_pulse(uint16_t tempo) {
  return (uint32_t)((SEC_PER_MIN * MS_PER_SEC * MICROS_PER_MS) / (float)(PPQN * tempo));
}

Context::Context(Adafruit_NeoTrellisM4* trellis, Layout* initial) {
  _trellis = trellis;
  _layout = initial;
  setTempo(120);
}

Adafruit_NeoTrellisM4* Context::trellis() {
  return _trellis;
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

void Context::changeToNextLayout() {
  _layout = _layout->next();
  refresh();
}
