#include <Arduino.h>
#include "constants.h"
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

//TODO move
class Sequence {
  public:
    virtual void update(Context *context) = 0;
    virtual byte value() = 0;
    virtual void start();
    virtual void stop();
};

//TODO move
class LFOSequence : public Sequence {
  private:
    boolean enabled;
    LFO *lfo;
    byte cc;
    byte minVal;
    byte maxVal;
    byte _value;

  public:
    LFOSequence(LFO* lfo, byte cc, byte minVal, byte maxVal);
    void update(Context *context);
    byte value();
    void start();
    void stop();
};

class LFOButton : public Button {
  private:
    boolean _active;
    LFOSequence _seq = LFOSequence(NULL, 0, 0, 127);
    byte cc;
    LFO* lfo;
  public:
    LFOButton(byte cc, LFO* lfo);
    uint32_t on_color();
    uint32_t off_color();
    void update(byte key, Context* context);
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
};

class LFOTypeButton : public Button {
  private:
    LFO* _lfo;
    byte currentIndex = 0;
  public:
    LFOTypeButton(LFO *lfo);
    uint32_t on_color();
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
    void update(byte key, Context* context);
};

class LFOSpeedButton : public Button {
  private:
    LFO* _lfo;
    byte currentIndex = 0;
    byte all[9]={4*PPQN, 3*PPQN, 2*PPQN, PPQN, PPQN/2, 5*PPQN, 6*PPQN, 7*PPQN, 8*PPQN};
  public:
    LFOSpeedButton(LFO *lfo);
    uint32_t on_color();
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
    void update(byte key, Context* context);
};

class LFOResetButton : public Button {
  private:
    LFO* _x_lfo;
    LFO* _y_lfo;
  public:
    LFOResetButton(LFO* xLFO, LFO* yLFO);
    uint32_t on_color();
    void pressed(byte key, Context* context);
};
