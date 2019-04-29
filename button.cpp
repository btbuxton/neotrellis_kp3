#include "button.h"
#include "color.h"

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

void Button::pressed(byte key, Adafruit_NeoTrellisM4* trellis) {
  _pressed = true;
  trellis->setPixelColor(key, on_color());
}

void Button::released(byte key, Adafruit_NeoTrellisM4* trellis) {
  _pressed = false;
  trellis->setPixelColor(key, off_color());
}

void Button::refresh(byte key, Adafruit_NeoTrellisM4* trellis) {
  uint32_t color = off_color();
  if (_pressed) {
    color = on_color();
  }
  trellis->setPixelColor(key, color);
}

void Button::group_pressed(Adafruit_NeoTrellisM4* trellis) {
}

void Button::group_released(Adafruit_NeoTrellisM4* trellis) {
}

//class TempoButton
TempoButton::TempoButton(uint32_t value) : Button() {
  _value = value;
  _group = 1;
}

uint32_t TempoButton::on_color() {
  return GREEN;
}

void TempoButton::pressed(byte key, Adafruit_NeoTrellisM4* trellis) {
  Button::pressed(key, trellis);
  // TODO - Pass in context instead?
//  CURRENT_TEMPO = _value;
//  CURRENT_DELAY = micros_per_pulse(CURRENT_TEMPO);
}

//class XButton
XButton::XButton() : Button() {
  _group = 2;
}

uint32_t XButton::on_color() {
  return BLUE;
}

void XButton::pressed(byte key, Adafruit_NeoTrellisM4* trellis) {
  Button::pressed(key, trellis);
  byte value = CC_VALUES[key % 8];
  trellis->controlChange(12, value);
}

void XButton::group_pressed(Adafruit_NeoTrellisM4* trellis) {
  trellis->controlChange(92,0xFF);
}

void XButton::group_released(Adafruit_NeoTrellisM4* trellis) {
  trellis->controlChange(92,0x00);
}

//class YButton : public Button {
YButton::YButton() : Button() {
  _group = 3;
}
uint32_t YButton::on_color() {
  return YELLOW;
}

void YButton::pressed(byte key, Adafruit_NeoTrellisM4* trellis) {
  Button::pressed(key, trellis);
  byte value = CC_VALUES[key % 8];
  trellis->controlChange(13, value);
}

void YButton::group_pressed(Adafruit_NeoTrellisM4* trellis) {
  trellis->controlChange(92,0xFF);
}

void YButton::group_released(Adafruit_NeoTrellisM4* trellis) {
  trellis->controlChange(92,0x00);
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

void NoteButton::pressed(byte key, Adafruit_NeoTrellisM4* trellis) {
  Button::pressed(key, trellis);
  trellis->controlChange(12, _value);
}

void NoteButton::group_pressed(Adafruit_NeoTrellisM4* trellis) {
  trellis->controlChange(92,0xFF);
}

void NoteButton::group_released(Adafruit_NeoTrellisM4* trellis) {
  trellis->controlChange(92,0x00);
}
