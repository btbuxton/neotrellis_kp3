#include <Adafruit_NeoTrellisM4.h>
#include "button.h"
#include "color.h"

#define MIDI_CHANNEL 0

#define PPQN 24
#define MS_PER_SEC 1000
#define MICROS_PER_MS 1000
#define SEC_PER_MIN 60

#define CLOCK_MSG 0xF8

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

class Layout {
  protected:
    Button *_all[32];
    Layout *_next;
  public:
    Layout() {  
    }
    
    Button **all() {
      return _all;
    }

    void refresh() {
      for (byte i=0; i < 32; i++) {
        _all[i]->refresh(i, &TRELLIS);
      }
    }

    Layout* next() {
      return _next;
    }

    void setNext(Layout *layout) {
      _next = layout;
    }
};

static Layout *CURRENT_LAYOUT;

class NextLayoutButton : public Button {
  private:
    Layout *_next;

  public:
    NextLayoutButton() : Button() {
    }
    
    uint32_t on_color() {
      return RED;
    }
    
    void released(byte key, Adafruit_NeoTrellisM4* trellis) {
      Button::released(key, trellis);
      Serial.println(key);
      Serial.println((uint32_t)_next);
      CURRENT_LAYOUT = _next;
      CURRENT_LAYOUT->refresh();
    }
    
    Layout* next() {
      return _next;
    }

    void setNext(Layout *layout) {
      _next = layout;
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
    Button misc[10];
    NextLayoutButton nextLayout;
    NoteButton notes[13];
    YButton yButtons[8];
  public:
    OneOctaveLayout() : Layout() {
      for (byte i=0; i< 13; i++) {
        notes[i].value(ONE_OCT_VALUES[i]);
      }
      _all[0] = &nextLayout;
      _all[1] = &notes[1];
      _all[2] = &notes[3];
      _all[3] = &misc[0];
      _all[4] = &notes[6];
      _all[5] = &notes[8];
      _all[6] = &notes[10];
      _all[7] = &misc[1];
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
        _all[i] = &misc[i - 22];
      }
    }
    void setNext(Layout *layout) {
      Layout::setNext(layout);
      nextLayout.setNext(layout);
    }
    
};

class DefaultLayout: public Layout {
  private:
    Button misc[12];
    NextLayoutButton nextLayout;
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
      } else if (i == 16) {
        _all[i] = &nextLayout;
      } else if (i < 29) {
        _all[i] = &misc[i - 17];
      } else {
        _all[i] = &tempoButtons[i - 29];
      }
    }
  }

  //YUCK - get rid of this
  void setNext(Layout *layout) {
      Layout::setNext(layout);
      nextLayout.setNext(layout);
  }

};

static DefaultLayout DEFAULT_LAYOUT = DefaultLayout();
static OneOctaveLayout ONE_OCTAVE_LAYOUT = OneOctaveLayout();

void setup() {
  Serial.begin(115200);
  Serial.println("kp3+ berzerker");
  TRELLIS.begin();
  TRELLIS.enableUARTMIDI(true);
  TRELLIS.setUARTMIDIchannel(MIDI_CHANNEL);
  CURRENT_DELAY = micros_per_pulse(CURRENT_TEMPO);
  
  DEFAULT_LAYOUT.setNext(&ONE_OCTAVE_LAYOUT);
  ONE_OCTAVE_LAYOUT.setNext(&DEFAULT_LAYOUT);
  CURRENT_LAYOUT = &DEFAULT_LAYOUT;
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
      state->pressed(key, &TRELLIS);
      if (++pressed_count[state->group()] > 0) {
        state->group_pressed(&TRELLIS);
      }
    } else if (e.bit.EVENT == KEY_JUST_RELEASED) {
      state->released(key, &TRELLIS);
      if (--pressed_count[state->group()] == 0) {
        state->group_released(&TRELLIS);
      }
    }
//    for (byte i=0; i < TRELLIS.num_keys(); i++) {
//      if (ALL[i]->isPressed()) {
//        //Serial.println(i);
//      }
//    }
  }
  //Serial1 is midi
  Serial1.write(CLOCK_MSG); //clock message
  TRELLIS.sendMIDI();
  clock_delay(CURRENT_DELAY);
}

// TODO
// fix tempo button - context?
// Break out -> layout
// Panic button
// Refactor groups - move to layout - move loop to layout as well
// Two Octave Layout
// Accelerometer for x/y values (new button)
// Sample turn/off
// Mute/Hold/other buttons?
// LFO between values ( 2 pressed )
// Arp for notes (or between values of more than 2 pressed)
