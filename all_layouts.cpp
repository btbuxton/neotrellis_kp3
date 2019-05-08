#include "all_layouts.h"
#include "context.h"
#include "constants.h"

void CommonLayout::pressed(byte key, Context* context) {
  Layout::pressed(key, context);
  byte group = _all[key]->group();
  if (group == 0 || group > 3) {
    return;
  }
  byte count = ++_group_count[group];
  if (count > 0) {
    group_pressed(group, count, context);
  }
}

void CommonLayout::released(byte key, Context* context) {
  Layout::released(key, context);
  byte group = _all[key]->group();
  if (group == 0 || group > 3) {
    return;
  }
  byte count = --_group_count[group];
  if (count == 0) {
    group_released(group, count, context);
  }
}

void CommonLayout::group_pressed(byte group, byte count, Context* context) {
  context->trellis()->controlChange(CC_TOUCH,0xFF);
}

void CommonLayout::group_released(byte group, byte count, Context* context) {
  if (group == 1) {
    context->trellis()->controlChange(CC_TOUCH,0x00);
  }
}
    
DefaultLayout::DefaultLayout() : CommonLayout() {
  _all[0] = &nextLayout;
  _all[1] = &misc[0];
  _all[2] = &misc[1];
  _all[3] = &misc[2];
  _all[4] = &misc[3];
  _all[5] = &misc[4];
  _all[6] = &misc[5];
  _all[7] = &misc[6];
  _all[8] = &xButtons[0];
  _all[9] = &xButtons[1];
  _all[10] = &xButtons[2];
  _all[11] = &xButtons[3];
  _all[12] = &xButtons[4];
  _all[13] = &xButtons[5];
  _all[14] = &xButtons[6];
  _all[15] = &xButtons[7];
  _all[16] = &yButtons[0];
  _all[17] = &yButtons[1];
  _all[18] = &yButtons[2];
  _all[19] = &yButtons[3];
  _all[20] = &yButtons[4];
  _all[21] = &yButtons[5];
  _all[22] = &yButtons[6];
  _all[23] = &yButtons[7];
  _all[24] = &lfoButton;
  _all[25] = &lfoTypeButton;
  _all[26] = &lfoSpeedButton;
  _all[27] = &misc[7];
  _all[28] = &misc[8];
  _all[29] = &tempoButtons[0];
  _all[30] = &tempoButtons[1];
  _all[31] = &tempoButtons[2];
}

/* -  C# D# -  F# G# A# -
 * C  D  E  F  G  A  B  C
 * 
 * FF 01 03 FF 06 08 0A FF
 * 00 02 04 05 07 09 0B 0C
 */

//starts on C - the synths are 1 octave
static const byte ONE_OCT_VALUES[] = {0,11,20,29,38,47,56,65,75,84,93,102,120};

OneOctaveLayout::OneOctaveLayout() : CommonLayout() {
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
  _all[24] = &accelXY;
  _all[25] = &samples[0];
  _all[26] = &samples[1];
  _all[27] = &samples[2];
  _all[28] = &samples[3];
  _all[29] = &misc[2];
  _all[30] = &misc[3];
  _all[31] = &misc[4];
}

/* -  C# D# -  F# G# A# -
 * C  D  E  F  G  A  B  C
 * 
 * FF 01 03 FF 06 08 0A FF
 * 00 02 04 05 07 09 0B 0C
 * 
 * FF 0D 0F FF 12 14 16 FF
 * 0C 0E 10 11 13 15 17 18
 */

//starts on C - the vocoders are 2 octave
static const byte TWO_OCT_VALUES[] = {0,6,11,16,21,26,31,36,41,46,51,56,61,65,70,75,80,90,95,100,105,110,115,120,125};

TwoOctaveLayout::TwoOctaveLayout() : CommonLayout() {
  for (byte i=0; i<25; i++) {
    notes[i].value(TWO_OCT_VALUES[i]);
  }
  
  _all[0] = &nextLayout;
  _all[1] = &notes[1];
  _all[2] = &notes[3];
  _all[3] = &samples[0];
  _all[4] = &notes[6];
  _all[5] = &notes[8];
  _all[6] = &notes[10];
  _all[7] = &samples[1];
  _all[8] = &notes[0];
  _all[9] = &notes[2];
  _all[10] = &notes[4];
  _all[11] = &notes[5];
  _all[12] = &notes[7];
  _all[13] = &notes[9];
  _all[14] = &notes[11];
  _all[15] = &notes[12];
  
  _all[16] = &accelXY;
  _all[17] = &notes[13];
  _all[18] = &notes[15];
  _all[19] = &samples[2];
  _all[20] = &notes[18];
  _all[21] = &notes[20];
  _all[22] = &notes[22];
  _all[23] = &samples[3];
  _all[24] = &notes[12];
  _all[25] = &notes[14];
  _all[26] = &notes[16];
  _all[27] = &notes[17];
  _all[28] = &notes[19];
  _all[29] = &notes[21];
  _all[30] = &notes[23];
  _all[31] = &notes[24];
}
