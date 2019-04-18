#include "GPIO.h"
#include "Arduino.h"
class mux {
  public:
    mux() {
      pinMode(MUX_SEL_A_PIN, OUTPUT);
      pinMode(MUX_SEL_B_PIN, OUTPUT);
    };
    ~mux(){};
};

