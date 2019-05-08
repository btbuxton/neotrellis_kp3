#include <Arduino.h>
#include "lfo.h"

class Context;

class Button {    
  protected:
    byte _group;
    
  public:
    Button();   
    byte group();
    virtual uint32_t on_color();
    virtual uint32_t off_color();
    virtual void update(byte key, Context* context);
    virtual void pressed(byte key, Context* context);
    virtual void released(byte key, Context* context);
    virtual void refresh(byte key, Context* context);
    virtual void cc_value(byte key, byte* cc, byte* value);
};

class TempoButton : public Button {
    private:
      uint32_t _value;
      
    public:
      TempoButton(uint32_t value);
      uint32_t on_color();
      void pressed(byte key, Context* context);
};

class XButton : public Button {
  public:
      XButton();
      uint32_t on_color();
      void pressed(byte key, Context* context);
      void cc_value(byte key, byte* cc, byte* value);
};

class YButton : public Button {
  public:
      YButton();
      uint32_t on_color();
      void pressed(byte key, Context* context);
};

class NoteButton : public Button {
  private:
      byte _value;
  public:
      NoteButton();
      void value(byte new_value);
      uint32_t on_color();
      void pressed(byte key, Context* context);
      void cc_value(byte key, byte* cc, byte* value);
};

class PlayButton : public Button {
  private:
    byte _value;
  public:
    PlayButton(byte value);
    uint32_t on_color();
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
};

class AccelButton : public Button {
  public:
    AccelButton();
    uint32_t on_color();
    void update(byte key, Context* context);
};

class NextLayoutButton : public Button {
  public:
    NextLayoutButton();
    uint32_t on_color();
    void released(byte key, Context *context);
};

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

class LFOButton : public Button {
  private:
    boolean _active;
    WaveSequence _seq = WaveSequence(NULL, 0, 0, 127);
    byte cc;
    Wave **wave;
  public:
    LFOButton(byte cc, Wave** wave);
    uint32_t on_color();
    uint32_t off_color();
    void update(byte key, Context* context);
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
};

class LFOTypeButton : public Button {
  private:
    SineWave sine;
    SquareWave square;
    SawWave saw;
    SawWave up;
    SawWave innerDown;
    NegWave down = NegWave(&innerDown);
    Wave* waves[4] = {&sine, &square, &up,&down};
    byte currentIndex = 0;
  public:
    Wave *current = waves[currentIndex];
    uint32_t on_color();
    void released(byte key, Context* context);
};
