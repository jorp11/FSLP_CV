#ifndef DAC_H_
#define DAC_H_
#include "Arduino.h"
enum DAC_CHANNEL_t {
  DAC_CHANNEL_A,
  DAC_CHANNEL_B,
  DAC_CHANNEL_C,
  DAC_CHANNEL_D
};

class dac {
  public:
    dac();
    ~dac() {};
    void dacWrite(uint16_t data, DAC_CHANNEL_t channel);

  private:


};
#endif
