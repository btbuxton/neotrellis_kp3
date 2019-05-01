#include <Arduino.h>

class Context;

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
    virtual void update(byte key, Context* context);
    virtual void pressed(byte key, Context* context);
    virtual void released(byte key, Context* context);
    virtual void refresh(byte key, Context* context);
    virtual void group_pressed(Context* context);
    virtual void group_released(Context* context);
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
      virtual void group_pressed(Context* context);
      virtual void group_released(Context* context);
};

class YButton : public Button {
  public:
      YButton();
      uint32_t on_color();
      void pressed(byte key, Context* context);
      virtual void group_pressed(Context* context);
      virtual void group_released(Context* context);
};

class NoteButton : public Button {
  private:
      byte _value;
  public:
      NoteButton();
      void value(byte new_value);
      uint32_t on_color();
      void pressed(byte key, Context* context);
      virtual void group_pressed(Context* context);
      virtual void group_released(Context* context);
};

class PlayButton : public Button {
  private:
    byte _value;
  public:
    PlayButton();
    void value(byte new_value);
    uint32_t on_color();
    void pressed(byte key, Context* context);
    void released(byte key, Context* context);
};

class AccelButton : public Button {
  public:
    AccelButton();
    uint32_t on_color();
    void update(byte key, Context* context);
    void group_pressed(Context* context);
    void group_released(Context* context);
};

class NextLayoutButton : public Button {
  public:
    NextLayoutButton();
    uint32_t on_color();
    void released(byte key, Context *context);
};
