/**
 * Context is the manager of the layouts and allows them to be switched. 
 * It also holds common objects needed throughout project.
 * This is where common actions (like issuing an update for all) is placed.
 */
#include <Adafruit_NeoTrellisM4.h>

class Adafruit_ADXL343;
class Layout;

class Context {
  private:
    Adafruit_NeoTrellisM4* _trellis;
    Adafruit_ADXL343* _accel;
    Layout* _layout;
    uint16_t _tempo;
    uint32_t _clockDelay;
    
  public:
    Context(Adafruit_NeoTrellisM4* trellis, Adafruit_ADXL343* accel, Layout* initial);   
    Adafruit_NeoTrellisM4* trellis();
    Adafruit_ADXL343* accel();
    Layout* layout();
    void setLayout(Layout* newLayout);
    uint16_t tempo();
    void setTempo(uint16_t newTempo);
    uint32_t clockDelay();
    void refresh();
    void update();
    void changeToNextLayout();
};
