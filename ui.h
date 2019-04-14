#ifndef UI_H_
#define UI_H_
#include "Arduino.h"
#include "GPIO.h"


enum UI_MODE {
  UI_MODE_NORM,
  UI_MODE_LOOP_REC,
  UI_MODE_LOOP_PLAY,
  UI_MODE_LATCH
};
class ui {
  public:

    ui() {
      _current_mode = UI_MODE_NORM;
    };
    ~ui() {};

    void setMode(UI_MODE newMode) {
      _current_mode = newMode;
    };
    UI_MODE getMode() {
      return _current_mode;
    };
    void doStuff();
  private:
    UI_MODE _current_mode;

};
#endif
