#include <Adafruit_NeoTrellisM4.h>

#define MIDI_CHANNEL 0

#define BLACK Adafruit_NeoPixel::Color(0,0,0)
#define RED Adafruit_NeoPixel::Color(0x22,0,0)
#define GREEN Adafruit_NeoPixel::Color(0,0x22,0)
#define BLUE Adafruit_NeoPixel::Color(0,0,0x22)
#define YELLOW Adafruit_NeoPixel::Color(0x22,0x22,0)
#define PURPLE Adafruit_NeoPixel::Color(0x22,0,0x22)

#define PPQN 24
#define MS_PER_SEC 1000
#define MICROS_PER_MS 1000
#define SEC_PER_MIN 60

static const byte CC_VALUES[] = {0x00, 0x10, 0x20, 0x30, 0x4f, 0x5F, 0x6F, 0x7F};

//starts on C - the vocoders are 2 octave
static const byte TWO_OCT_VALUES[] = {0,6,11,16,21,26,31,36,41,46,51,56,61,65,70,75,80,90,95,100,105,110,115,120,125};
//starts on C - the synths are 1 octave
static const byte ONE_OCT_VALUES[] = {0,11,20,29,38,47,56,65,75,84,93,102,120};

static Adafruit_NeoTrellisM4 TRELLIS = Adafruit_NeoTrellisM4();
static uint16_t CURRENT_TEMPO = 120;
static uint32_t CURRENT_DELAY = 10000;

uint32_t micros_per_pulse(uint16_t tempo) {
  return (uint32_t)((SEC_PER_MIN * MS_PER_SEC * MICROS_PER_MS) / (float)(PPQN * tempo));
}

// This is still off by 0.5 bpm - rounding/etc
void clock_delay(uint32_t delay_micros) {
  uint32_t now = micros();
  static uint32_t then = now;
  uint32_t diff;
  if (now >= then) {
    diff = now - then;
  } else {
    diff = 0xFFFFFFFF - then + now;
  }
  if (diff <= delay_micros) {
    delayMicroseconds(delay_micros - diff);
  } else {
    Serial.print("Failing behind! "); Serial.println(diff - delay_micros);
  }
  then = micros();
}

class Button {
  private:
    boolean _pressed;
  protected:
    byte _group;
    
  public:
    Button() {
      _pressed = false;
      _group = 0;
    }
    
    boolean isPressed() {
      return _pressed;
    }
    
    byte group() {
      return _group;
    }
    
    virtual uint32_t on_color() {
      return BLACK;
    }
    
    virtual uint32_t off_color() {
      return on_color() & 0x020202;
    }
    
    virtual void pressed(byte key) {
      _pressed = true;
      TRELLIS.setPixelColor(key, on_color());
    }
    
    virtual void released(byte key) {
      _pressed = false;
      TRELLIS.setPixelColor(key, off_color());
    }

    virtual void refresh(byte key) {
      uint32_t color = off_color();
      if (_pressed) {
        color = on_color();
      }
      TRELLIS.setPixelColor(key, color);
    }
    
    virtual void group_pressed() {
    }
    
    virtual void group_released() {
    }
};

class TempoButton : public Button {
    private:
      uint32_t _value;
      
    public:
      TempoButton(uint32_t value) : Button() {
        _value = value;
        _group = 1;
      }
      
      uint32_t on_color() {
        return GREEN;
      }
      
      void pressed(byte key) {
        Button::pressed(key);
        CURRENT_TEMPO = _value;
        CURRENT_DELAY = micros_per_pulse(CURRENT_TEMPO);
      }
};

class XButton : public Button {
  public:
      XButton() : Button() {
        _group = 2;
      }
      
      uint32_t on_color() {
        return BLUE;
      }
      
      void pressed(byte key) {
        Button::pressed(key);
        byte value = CC_VALUES[key % 8];
        TRELLIS.controlChange(12, value);
        //Serial.println(value);
      }

      void group_pressed() {
        TRELLIS.controlChange(92,0xFF);
      }

      void group_released() {
        TRELLIS.controlChange(92,0x00);
      }
};

class YButton : public Button {
  public:
      YButton() : Button() {
        _group = 3;
      }
      uint32_t on_color() {
        return YELLOW;
      }
      
      void pressed(byte key) {
        Button::pressed(key);
        byte value = CC_VALUES[key % 8];
        TRELLIS.controlChange(13, value);
      }

      void group_pressed() {
        TRELLIS.controlChange(92,0xFF);
      }

      void group_released() {
        TRELLIS.controlChange(92,0x00);
      }
};

class NoteButton : public Button {
  private:
      byte _value;
  public:
      NoteButton() : Button() {
        _group = 4;
        _value = 0;
      }
      
      void value(byte new_value) {
        _value = new_value;
      }
      
      uint32_t on_color() {
        return PURPLE;
      }
      
      void pressed(byte key) {
        Button::pressed(key);
        TRELLIS.controlChange(12, _value);
      }

      void group_pressed() {
        TRELLIS.controlChange(92,0xFF);
      }

      void group_released() {
        TRELLIS.controlChange(92,0x00);
      }
};

class Layout {
  protected:
    Button *_all[32];
  public:
    Layout() {  
    }
    
    Button **all() {
      return _all;
    }

    void refresh() {
      for (byte i=0; i < 32; i++) {
        _all[i]->refresh(i);
      }
    }
};


/* -  C# D# -  F# G# A# -
 * C  D  E  F  G  A  B  C
 * 
 * FF 01 03 FF 06 08 0A FF
 * 00 02 04 05 07 09 0B 0C
 */
 
class OneOctaveLayout: public Layout {
  private:
    Button misc[11];
    NoteButton notes[13];
    YButton yButtons[8];
  public:
    OneOctaveLayout() : Layout() {
      for (byte i=0; i< 13; i++) {
        notes[i].value(ONE_OCT_VALUES[i]);
      }
      _all[0] = &misc[0];
      _all[1] = &notes[1];
      _all[2] = &notes[3];
      _all[3] = &misc[1];
      _all[4] = &notes[6];
      _all[5] = &notes[8];
      _all[6] = &notes[10];
      _all[7] = &misc[2];
      _all[8] = &notes[0];
      _all[9] = &notes[2];
      _all[10] = &notes[4];
      _all[11] = &notes[5];
      _all[12] = &notes[7];
      _all[13] = &notes[9];
      _all[14] = &notes[11];
      _all[15] = &notes[12];
      
      for (byte i=16; i < 24; i++) {
        _all[i] = &yButtons[i - 16];
      }
      for (byte i=24; i < 32; i++) {
        _all[i] = &misc[i - 21];
      }
    }
};

class DefaultLayout: public Layout {
  private:
    Button misc[13];
    XButton xButtons[8];
    YButton yButtons[8];
    TempoButton tempoButtons[3] = {TempoButton(60), TempoButton(90), TempoButton(120)};

  public:
  DefaultLayout() : Layout() {
    for (byte i=0; i < 32; i++) {
      if (i < 8) {
        _all[i] = &xButtons[i];
      } else if (i < 16) {
        _all[i] = &yButtons[i - 8];
      } else if (i < 29) {
        _all[i] = &misc[i - 16];
      } else {
        _all[i] = &tempoButtons[i - 29];
      }
    }
  }

};

static DefaultLayout DEFAULT_LAYOUT = DefaultLayout();
static OneOctaveLayout ONE_OCTAVE_LAYOUT = OneOctaveLayout();
static Layout *CURRENT_LAYOUT;

void setup() {
  Serial.begin(115200);
  Serial.println("kp3+ berzerker");
  TRELLIS.begin();
  TRELLIS.enableUARTMIDI(true);
  TRELLIS.setUARTMIDIchannel(MIDI_CHANNEL);
  CURRENT_DELAY = micros_per_pulse(CURRENT_TEMPO);
  //CURRENT_LAYOUT = &DEFAULT_LAYOUT;
  CURRENT_LAYOUT = &ONE_OCTAVE_LAYOUT;
  CURRENT_LAYOUT->refresh();
}

void loop() {
  static byte pressed_count[] = {0,0,0,0,0};
  TRELLIS.tick();
  uint32_t color = Adafruit_NeoPixel::Color(0,0,0x20);
  while (TRELLIS.available()){
    keypadEvent e = TRELLIS.read();
    byte key = e.bit.KEY;
    Button **all = CURRENT_LAYOUT->all();
    Button* state = all[key];
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      state->pressed(key);
      if (++pressed_count[state->group()] > 0) {
        state->group_pressed();
      }
    } else if (e.bit.EVENT == KEY_JUST_RELEASED) {
      state->released(key);
      if (--pressed_count[state->group()] == 0) {
        state->group_released();
      }
    }
//    for (byte i=0; i < TRELLIS.num_keys(); i++) {
//      if (ALL[i]->isPressed()) {
//        //Serial.println(i);
//      }
//    }
  }
  //Serial1 is midi
  Serial1.write(0xF8); //clock message
  TRELLIS.sendMIDI();
  clock_delay(CURRENT_DELAY);
}

// TODO
// Two Octave Layout
// Layout Switch
// Accelerometer for x/y values (new button)
// Sample turn/off
// LFO between values ( 2 pressed )
// Arp for notes (or between values of more than 2 pressed)
