#include "buttons.h"
#include "Arduino.h"
#include "GPIO.h"
buttons::buttons() {
    pinMode(LOOP_BUTTON_PIN,INPUT_PULLUP);
    pinMode(LATCH_BUTTON_PIN,INPUT_PULLUP);
}

void buttons::debounce() {
    const uint16_t pins[] = { LATCH_BUTTON_PIN, LOOP_BUTTON_PIN};

    for (int i =0; i<kNumButtons;++i){
    _button_state[i] = (_button_state[i] << 1) | \
        digitalRead(pins[i]);
  }
}

