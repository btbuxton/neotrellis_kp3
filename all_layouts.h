#include "button.h"
#include "layout.h"

class DefaultLayout: public Layout {
  private:
    Button misc[12];
    NextLayoutButton nextLayout;
    XButton xButtons[8];
    YButton yButtons[8];
    TempoButton tempoButtons[3] = {TempoButton(60), TempoButton(90), TempoButton(120)};
  public:
    DefaultLayout();
};

class OneOctaveLayout: public Layout {
  private:
    Button misc[10];
    NextLayoutButton nextLayout;
    NoteButton notes[13];
    YButton yButtons[8];
  public:
    OneOctaveLayout();
};

class TwoOctaveLayout: public Layout {
  private:
    Button misc[1];
    PlayButton samples[4];
    NextLayoutButton nextLayout;
    NoteButton notes[25];
  public:
    TwoOctaveLayout();
};
