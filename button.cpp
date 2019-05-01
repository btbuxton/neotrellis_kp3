#include <Adafruit_ADXL343.h>
#include "button.h"
#include "color.h"
#include "layout.h"
#include "context.h"

static const byte CC_VALUES[] = {0x00, 0x10, 0x20, 0x30, 0x4f, 0x5F, 0x6F, 0x7F};

Button::Button() {
  _pressed = false;
  _group = 0;
}
    
boolean Button::isPressed() {
  return _pressed;
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
  _pressed = true;
  context->trellis()->setPixelColor(key, on_color());
}

void Button::released(byte key, Context* context) {
  _pressed = false;
  context->trellis()->setPixelColor(key, off_color());
}

void Button::refresh(byte key, Context* context) {
  uint32_t color = off_color();
  if (_pressed) {
    color = on_color();
  }
  context->trellis()->setPixelColor(key, color);
}

void Button::group_pressed(Context* context) {
}

void Button::group_released(Context* context) {
}

//class TempoButton
TempoButton::TempoButton(uint32_t value) : Button() {
  _value = value;
  _group = 1;
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
  _group = 2;
}

uint32_t XButton::on_color() {
  return BLUE;
}

void XButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  byte value = CC_VALUES[key % 8];
  context->trellis()->controlChange(12, value);
}

void XButton::group_pressed(Context* context) {
  context->trellis()->controlChange(92,0xFF);
}

void XButton::group_released(Context* context) {
  context->trellis()->controlChange(92,0x00);
}

YButton::YButton() : Button() {
  _group = 3;
}

uint32_t YButton::on_color() {
  return YELLOW;
}

void YButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  byte value = CC_VALUES[key % 8];
  context->trellis()->controlChange(13, value);
}

void YButton::group_pressed(Context* context) {
  context->trellis()->controlChange(92,0xFF);
}

void YButton::group_released(Context* context) {
  context->trellis()->controlChange(92,0x00);
}

NoteButton::NoteButton() : Button() {
  _group = 4;
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
  context->trellis()->controlChange(12, _value);
}

void NoteButton::group_pressed(Context* context) {
  context->trellis()->controlChange(92,0xFF);
}

void NoteButton::group_released(Context* context) {
  context->trellis()->controlChange(92,0x00);
}

PlayButton::PlayButton() : Button() {
  _value = 0;
}

void PlayButton::value(byte new_value) {
  _value = new_value;
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
  _group = 3;
}

uint32_t AccelButton::on_color() {
  return ORANGE;
}

byte convert_accel_value(float value) {
  return (byte)((abs(value) / 11.0) * 0x7f) & 0x7F;
}

void AccelButton::update(byte key, Context* context) {
  Button::update(key, context);
  if (!isPressed()) {
    return;
  }
  sensors_event_t event;
  context->accel()->getEvent(&event);
  float x_value = event.acceleration.y;
  float y_value = event.acceleration.x;
  context->trellis()->controlChange(12, convert_accel_value(x_value));
  context->trellis()->controlChange(12, convert_accel_value(y_value));
}

void AccelButton::group_pressed(Context* context) {
  context->trellis()->controlChange(92,0xFF);
}

void AccelButton::group_released(Context* context) {
  context->trellis()->controlChange(92,0x00);
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
