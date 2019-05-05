#include "button.h"
#include "layout.h"
#include "constants.h"
#include "lfo.h"

class Sequence {
  public:
    virtual void update(Context *context) = 0;
};

class WaveSequence : public Sequence {
  private:
    Wave *wave;
    byte cc;
    byte minVal;
    byte maxVal;

  public:
    WaveSequence(Wave* wave, byte cc, byte minVal, byte maxVal);
    void update(Context *context);
};

class CommonLayout : public Layout {
  protected:
    byte _group_count[4] = {0,0,0,0};
    Sequence* _seq[4]={NULL,NULL,NULL,NULL};
    virtual void lfoBetweenValues(byte cc, byte minValue, byte maxvalue);
  public:
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
    void update(Context* context);
    virtual void group_pressed(byte group, byte count, Context* context);
    virtual void group_released(byte group, byte count, Context* context);
};

class DefaultLayout: public CommonLayout {
  private:
    Button misc[12];
    NextLayoutButton nextLayout;
    XButton xButtons[8];
    YButton yButtons[8];
    TempoButton tempoButtons[3] = {TempoButton(60), TempoButton(90), TempoButton(120)};
  public:
    DefaultLayout();
};

class OneOctaveLayout: public CommonLayout {
  private:
    Button misc[5];
    NextLayoutButton nextLayout;
    NoteButton notes[13];
    YButton yButtons[8];
    AccelButton accelXY = AccelButton();
    PlayButton samples[4] = {PlayButton(NOTE_A), PlayButton(NOTE_B), PlayButton(NOTE_C), PlayButton(NOTE_D)};
  public:
    OneOctaveLayout();
};

class TwoOctaveLayout: public CommonLayout {
  private:
    AccelButton accelXY = AccelButton();
    PlayButton samples[4] = {PlayButton(NOTE_A), PlayButton(NOTE_B), PlayButton(NOTE_C), PlayButton(NOTE_D)};
    NextLayoutButton nextLayout;
    NoteButton notes[25];
  public:
    TwoOctaveLayout();
};
