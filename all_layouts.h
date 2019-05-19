/**
 * All layouts defined. Each class defines a layout for the buttons
 * it is recommended to keep the top left most button as the switcher.
 */

#include "button.h"
#include "layout.h"
#include "constants.h"

/**
 * Common code for each layout. It probably can be moved to Layout itself
 */
class CommonLayout : public Layout {
  protected:
    byte _group_count[4] = {0,0,0,0};
  public:
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
    //void update(Context* context);
    virtual void group_pressed(byte group, byte count, Context* context);
    virtual void group_released(byte group, byte count, Context* context);
};

/**
 * Layout with x,y buttons, etc. This was the initial playground.
 */
class DefaultLayout: public CommonLayout {
  private:
    LFO x_lfo;
    LFO y_lfo;
    
    NextLayoutButton nextLayout;
    XButton xButtons[8];
    YButton yButtons[8];
    
    LFOButton x_lfoButton = LFOButton(CC_X, &x_lfo);
    LFOTypeButton x_lfoTypeButton = LFOTypeButton(&x_lfo);
    LFOSpeedButton x_lfoSpeedButton = LFOSpeedButton(&x_lfo);

    LFOButton y_lfoButton = LFOButton(CC_Y, &y_lfo);
    LFOTypeButton y_lfoTypeButton = LFOTypeButton(&y_lfo);
    LFOSpeedButton y_lfoSpeedButton = LFOSpeedButton(&y_lfo);

    LFOResetButton lfoResetButton = LFOResetButton(&x_lfo, &y_lfo);

    PlayButton samples[4] = {PlayButton(NOTE_A), PlayButton(NOTE_B), PlayButton(NOTE_C), PlayButton(NOTE_D)};
    TempoButton tempoButtons[4] = {TempoButton(60), TempoButton(90), TempoButton(120), TempoButton(150)};
  public:
    DefaultLayout();
};

/**
 * Layout for the synth effect
 * A one octave keyboard with additional buttons
 */
class OneOctaveLayout: public CommonLayout {
  private:
    LFO y_lfo;
    Button blank;
    NextLayoutButton nextLayout;
    NoteButton notes[13];
    YButton yButtons[8];
    AccelButton accelXY = AccelButton();
    PlayButton samples[4] = {PlayButton(NOTE_A), PlayButton(NOTE_B), PlayButton(NOTE_C), PlayButton(NOTE_D)};

    LFOButton y_lfoButton = LFOButton(CC_Y, &y_lfo);
    LFOTypeButton y_lfoTypeButton = LFOTypeButton(&y_lfo);
    LFOSpeedButton y_lfoSpeedButton = LFOSpeedButton(&y_lfo);
  public:
    OneOctaveLayout();
};

/**
 * Two octave layout for the vocoder patches
 */
class TwoOctaveLayout: public CommonLayout {
  private:
    AccelButton accelXY = AccelButton();
    PlayButton samples[4] = {PlayButton(NOTE_A), PlayButton(NOTE_B), PlayButton(NOTE_C), PlayButton(NOTE_D)};
    NextLayoutButton nextLayout;
    NoteButton notes[25];
  public:
    TwoOctaveLayout();
};
