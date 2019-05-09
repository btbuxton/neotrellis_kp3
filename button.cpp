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

//TODO move
LFOSequence::LFOSequence(LFO* lfo, byte cc, byte minVal, byte maxVal) : Sequence() {
  this->enabled = false;
  this->lfo = lfo;
  this->cc = cc;
  this->minVal = minVal;
  this->maxVal = maxVal;
  this->_value = 0;
}

void LFOSequence::update(Context* context) {
  float scale = (lfo->next() + 1.0) / 2.0;
  byte len = maxVal - minVal;
  _value = minVal + (len * scale);
  //Serial.println(_value);
  if (enabled && cc > 0) {
    context->trellis()->controlChange(cc, _value);
  }
}

byte LFOSequence::value() {
  return _value;
}

void LFOSequence::start() {
  enabled = true;
}

void LFOSequence::stop() {
  enabled = false;
}

LFOButton::LFOButton(byte cc, LFO *lfo) : Button() {
  _active = false;
  this->cc = cc;
  this->lfo = lfo;
  this->_seq = LFOSequence(lfo,cc,0,127);
}

uint32_t LFOButton::on_color() {
  return RED | _seq.value();
}

uint32_t LFOButton::off_color() {
  if (_active) {
    return on_color();
  } else {
    return Button::off_color() | _seq.value();
  }
}

void LFOButton::update(byte key, Context* context) {
  Button::update(key, context);
//  if (!_active) {
//    return;
//  }
  _seq.update(context);
  uint32_t color = _active ? on_color() : off_color();
  context->trellis()->setPixelColor(key, color);
}

void LFOButton::pressed(byte key, Context* context) {
  Button::pressed(key, context);
  _active = !_active;
  if (_active) {
    _seq.start();
    context->trellis()->controlChange(CC_TOUCH,0xFF);
  } else {
    _seq.stop();
    context->trellis()->controlChange(CC_TOUCH,0x00);
  }
}

void LFOButton::released(byte key, Context* context) {
  Button::released(key, context);
  //context->trellis()->controlChange(CC_TOUCH,0x00);
}

LFOTypeButton::LFOTypeButton(LFO* lfo) {
  _lfo = lfo;
  _lfo->setWave(_lfo->all()[currentIndex]);
}

uint32_t LFOTypeButton::on_color() {
  switch(currentIndex) {
    //TODO CHANGE!!
    case 0 : return BLUE;
    case 1 : return RED;
    case 2 : return GREEN;
    case 3 : return YELLOW;
    default: return BLACK;
  }   
}

void LFOTypeButton::pressed(byte key, Context* context) {
  currentIndex = ++currentIndex % 4;
  Button::pressed(key, context);
}

void LFOTypeButton::released(byte key, Context* context) {
  _lfo->setWave(_lfo->all()[currentIndex]);
  Button::released(key, context);
}

void LFOTypeButton::update(byte key, Context* context) {
  Button::update(key, context);
  if (context->trellis()->isPressed(key)) {
    return;
  }
  uint32_t value = (byte)(((_lfo->value() + 1) / 2) * 0x3f);
  value = value << 16 | value << 8 | value; 
  context->trellis()->setPixelColor(key, off_color() | value);
}

LFOSpeedButton::LFOSpeedButton(LFO *lfo) {
  _lfo = lfo;
  _lfo->setLength(this->all[currentIndex]);
}

uint32_t LFOSpeedButton::on_color() {
  return GREEN;
}

void LFOSpeedButton::pressed(byte key, Context* context) {
  currentIndex = ++currentIndex % 9;
  Button::pressed(key, context);
}

void LFOSpeedButton::released(byte key, Context* context) {
  _lfo->setLength(all[currentIndex]);
  Button::released(key, context);
}

void LFOSpeedButton::update(byte key, Context* context) {
  Button::update(key, context);
  if (context->trellis()->isPressed(key)) {
    return;
  }
  uint32_t value = (byte)(((_lfo->value() + 1) / 2) * 0x3f);
  value = value << 16 | value << 8 | value; 
  context->trellis()->setPixelColor(key, off_color() | value);
}

LFOResetButton::LFOResetButton(LFO* xLFO, LFO* yLFO) : Button() {
  _x_lfo = xLFO;
  _y_lfo = yLFO;
}

uint32_t LFOResetButton::on_color() {
  return ORANGE;
}

void LFOResetButton::pressed(byte key, Context* context) {
  _x_lfo->reset();
  _y_lfo->reset();
}
    
