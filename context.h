#include <Adafruit_NeoTrellisM4.h>

class Layout;

class Context {
  private:
    Adafruit_NeoTrellisM4* _trellis;
    Layout* _layout;
    uint16_t _tempo;
    uint32_t _clockDelay;
    
  public:
    Context(Adafruit_NeoTrellisM4* trellis, Layout* initial);   
    Adafruit_NeoTrellisM4* trellis();
    Layout* layout();
    void setLayout(Layout* newLayout);
    uint16_t tempo();
    void setTempo(uint16_t newTempo);
    uint32_t clockDelay();
    void refresh();
    void changeToNextLayout();
};
