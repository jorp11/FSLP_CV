#include "GPIO.h"

class mux {
    mux() {
      pinMode(MUX_SEL_A_PIN, OUTPUT);
      pinMode(MUX_SEL_B_PIN, OUTPUT);
    };
    ~mux();
}

