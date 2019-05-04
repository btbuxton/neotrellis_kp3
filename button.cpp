#include <Adafruit_ADXL343.h>
#include "button.h"
#include "constants.h"
#include "layout.h"
#include "context.h"

static const byte CC_VALUES[] = {0x00, 0x10, 0x20, 0x30, 0x4f, 0x5F, 0x6F, 0x7F};

Button::Button() {
  _group = 0;
}

uint8_t Button::group() {
  return _group;
}

uint32_t Button::on_color() {
  return BLACK;
}

uint32_t Button::off_color() {
  return on_color() & 0x3F3F3F;
}

void Button::update(byte key, Context* context) {
  
}

void Button::pressed(byte key, Context* context) {
  context->trellis()->setPixelColor(key, on_color());
}

void Button::released(byte key, Context* context) {
  context->trellis()->setPixelColor(key, off_color());
}

void Button::refresh(byte key, Context* context) {
  uint32_t color = off_color();
  if (context->trellis()->isPressed(key)) {
    color = on_color();
  }
  context->trellis()->setPixelColor(key, color);
}

void Button::cc_value(byte key, byte* cc, byte* value) {
  *cc = 0;
  *value = 0;
}

//class TempoButton
TempoButton::TempoButton(uint32_t value) : Button() {
  _value = value;
  _group = 2;
}

uint32_t TempoButton::on_color() {
  return GREEN;
}

void TempoButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  context->setTempo(_value);
}

//class XButton
XButton::XButton() : Button() {
  _group = 1;
}

uint32_t XButton::on_color() {
  return BLUE;
}

void XButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  byte value = CC_VALUES[key % 8];
  context->trellis()->controlChange(CC_X, value);
}

void XButton::cc_value(byte key, byte* cc, byte* value) {
  *cc = CC_X;
  *value = CC_VALUES[key % 8];
}

YButton::YButton() : Button() {
  _group = 1;
}

uint32_t YButton::on_color() {
  return YELLOW;
}

void YButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  byte value = CC_VALUES[key % 8];
  context->trellis()->controlChange(CC_Y, value);
}

NoteButton::NoteButton() : Button() {
  _group = 1;
  _value = 0;
}

void NoteButton::value(byte new_value) {
  _value = new_value;
}

uint32_t NoteButton::on_color() {
  return PURPLE;
}

void NoteButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  context->trellis()->controlChange(CC_X, _value);
}

void NoteButton::cc_value(byte key, byte* cc, byte* value) {
  *cc = CC_X;
  *value = _value;
}

PlayButton::PlayButton(byte value) : Button() {
  _value = value;
}

uint32_t PlayButton::on_color() {
  return AQUA;
}

void PlayButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  context->trellis()->noteOn(_value, 0x7F);
}

void PlayButton::released(byte key, Context* context) {
  Button::released(key, context);
  context->trellis()->noteOff(_value, 0x00);
}

AccelButton::AccelButton() : Button() {
  _group = 1;
}

uint32_t AccelButton::on_color() {
  return ORANGE;
}

byte convert_accel_value(float value) {
  byte result;
  value = -value;
  if (value < 0) {
    result = (byte)((max(value, -10.0) / 10.0 * 0x3F) + 0x40);
  } else {
    result = (byte)((min(value, 10.0) / 10.0 * 0x3F) + 0x40);
  }
  //Serial.print(value); Serial.print(" "); Serial.println(result);
  return result;
}

void AccelButton::update(byte key, Context* context) {
  Button::update(key, context);
  if (!context->trellis()->isPressed(key)) {
    return;
  }
  sensors_event_t event;
  context->accel()->getEvent(&event);
  float x_value = event.acceleration.y;
  float y_value = event.acceleration.x;
  context->trellis()->controlChange(CC_X, convert_accel_value(x_value));
  context->trellis()->controlChange(CC_Y, convert_accel_value(y_value));
}

NextLayoutButton::NextLayoutButton() : Button() {
}

uint32_t NextLayoutButton::on_color() {
  return RED;
}

void NextLayoutButton::released(byte key, Context *context) {
  Button::released(key, context);
  context->changeToNextLayout();
}
