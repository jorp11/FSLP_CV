#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "Arduino.h"
#include "GPIO.h"
const uint8_t kNumButtons = 2;

class buttons {
  public:
    buttons();
    ~buttons(){};
    void debounce();

  inline bool released(uint8_t index) const {
    return _button_state[index] == 0x7f;
  }
  
  inline bool justPressed(uint8_t index) const {
    return _button_state[index] == 0x80;
  }
  inline bool justReleased(uint8_t index) const {
    return _button_state[index] == 0x01;
  }
  inline bool pressed(uint8_t index) const {
    return _button_state[index] == 0x00;
  }
  
  inline bool pressed_immediate(uint8_t index) const {
    if (index == 0){
      return !digitalRead(LATCH_BUTTON_PIN);
    } else {
    return !digitalRead(LOOP_BUTTON_PIN);
    }

  }

  private:
    uint8_t _button_state [kNumButtons];

};


#endif
