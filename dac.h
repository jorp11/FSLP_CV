//Header for TI DAC8565 driver


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
    void dacWriteChannel(uint16_t value, DAC_CHANNEL_t channel);
    void dacWriteChannels(uint16_t valueA, uint16_t valueB,uint16_t valueC,uint16_t valueD   );

  private:


};
#endif
