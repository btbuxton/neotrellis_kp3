#include <Arduino.h>
#include <Adafruit_NeoTrellisM4.h>

static const byte CC_VALUES[] = {0x00, 0x10, 0x20, 0x30, 0x4f, 0x5F, 0x6F, 0x7F};

class Button {
  private:
    boolean _pressed;
    
  protected:
    byte _group;
    
  public:
    Button();   
    boolean isPressed();
    byte group();
    virtual uint32_t on_color();
    virtual uint32_t off_color();
    virtual void pressed(byte key, Adafruit_NeoTrellisM4* trellis);
    virtual void released(byte key, Adafruit_NeoTrellisM4* trellis);
    virtual void refresh(byte key, Adafruit_NeoTrellisM4* trellis);
    virtual void group_pressed(Adafruit_NeoTrellisM4* trellis);
    virtual void group_released(Adafruit_NeoTrellisM4* trellis);
};

class TempoButton : public Button {
    private:
      uint32_t _value;
      
    public:
      TempoButton(uint32_t value);
      uint32_t on_color();
      void pressed(byte key, Adafruit_NeoTrellisM4* trellis);
};

class XButton : public Button {
  public:
      XButton();
      uint32_t on_color();
      void pressed(byte key, Adafruit_NeoTrellisM4* trellis);
      virtual void group_pressed(Adafruit_NeoTrellisM4* trellis);
      virtual void group_released(Adafruit_NeoTrellisM4* trellis);
};

class YButton : public Button {
  public:
      YButton();
      uint32_t on_color();
      void pressed(byte key, Adafruit_NeoTrellisM4* trellis);
      virtual void group_pressed(Adafruit_NeoTrellisM4* trellis);
      virtual void group_released(Adafruit_NeoTrellisM4* trellis);
};

class NoteButton : public Button {
  private:
      byte _value;
  public:
      NoteButton();
      void value(byte new_value);
      uint32_t on_color();
      void pressed(byte key, Adafruit_NeoTrellisM4* trellis);
      virtual void group_pressed(Adafruit_NeoTrellisM4* trellis);
      virtual void group_released(Adafruit_NeoTrellisM4* trellis);
};
