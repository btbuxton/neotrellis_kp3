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
  return on_color() & 0x020202;
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

//class YButton : public Button {
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

//class NoteButton
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
