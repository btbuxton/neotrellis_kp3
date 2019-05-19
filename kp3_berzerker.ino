/**
 * Controller for Korg KP3
 * Three layouts:
 * 1. Controls x,y via buttons, LFO, Tempo (midi clock), and samples
 * 2. Has one octave keyboard for synth effect, y, change x,y with accel an LFO, and play samples
 * 3.two octave keyboard for vocoder effect, change x,y with accel, and play samples
 * 
 * Made with Adafruit Trellis M4
 * Uses midi out via serial
 * 
 * This code was made as an experiment into MIDI, C++, and the Trellis.
 */


// TODO / Future Enhancements
// Refactor LFOSequence (move out of button.h/cpp)
// Use LFOSequence instead of LFO in buttons -> This allows changing min/max values
// Figure out how to set min/max values
// Panic button
// Implement Latch class for CC on/off ?! => interesting things happen now might not do
// Arp for notes (or between values of more than 2 pressed) => just use this to know max/min values of lfo

#include <Adafruit_NeoTrellisM4.h>
#include <Adafruit_ADXL343.h>
#include "all_layouts.h"
#include "context.h"

#define MIDI_CHANNEL 0

#define CLOCK_MSG 0xF8

static Adafruit_NeoTrellisM4 TRELLIS = Adafruit_NeoTrellisM4();
static Adafruit_ADXL343 ACCEL = Adafruit_ADXL343(123, &Wire1);

/**
 * Midi clock delay
 * Given delay in microsends, waits. It is expected to have thi called repeatedly. 
 * Takes difference of last call for wait.
 */
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

// Layout setup
static DefaultLayout DEFAULT_LAYOUT = DefaultLayout();
static OneOctaveLayout ONE_OCTAVE_LAYOUT = OneOctaveLayout();
static TwoOctaveLayout TWO_OCTAVE_LAYOUT = TwoOctaveLayout();
static Context CONTEXT = Context(&TRELLIS, &ACCEL, &DEFAULT_LAYOUT);

/**
 * Setup code to initialize the layouts and buttons
 */
void setup() {
  Serial.begin(115200);
  Serial.println("kp3+ berzerker");
  ACCEL.begin();
  ACCEL.setRange(ADXL343_RANGE_2_G); //can be 2, 4, 8, and 16
  
  TRELLIS.begin();
  TRELLIS.enableUARTMIDI(true);
  TRELLIS.setUARTMIDIchannel(MIDI_CHANNEL);
  TRELLIS.setBrightness(0x3F);
  
  DEFAULT_LAYOUT.setNext(&ONE_OCTAVE_LAYOUT);
  ONE_OCTAVE_LAYOUT.setNext(&TWO_OCTAVE_LAYOUT);
  TWO_OCTAVE_LAYOUT.setNext(&DEFAULT_LAYOUT);
  CONTEXT.refresh();
}

/**
 * Loop - Send out midi clock and read any keyboard events
 */
void loop() {
  TRELLIS.tick();
  CONTEXT.update();
  while (TRELLIS.available()) {
    keypadEvent e = TRELLIS.read();
    byte key = e.bit.KEY;
    Layout* layout = CONTEXT.layout();
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      layout->pressed(key, &CONTEXT);
    } else if (e.bit.EVENT == KEY_JUST_RELEASED) {
      layout->released(key, &CONTEXT);
    }
  }
  
  //Serial1 is midi
  Serial1.write(CLOCK_MSG); //clock message
  TRELLIS.sendMIDI();
  clock_delay(CONTEXT.clockDelay());
}
