#include <Arduino.h>

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
