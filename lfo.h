#ifndef LFO_H_
#define LFO_H_
#include "wavetables.h"
class lfo {
  public:
    lfo() {
      _wavetable= SINE_TABLE;
      _freq= 1;
    };
    ~lfo() {};
    uint16_t getVal();
    void setFreq(uint16_t);
  private:
    uint16_t _index;
    uint16_t _freq; //4095 corersponds to 10 hz
    const uint16_t *_wavetable;//= SINE_TABLE;
};
uint16_t lfo::getVal(){
  uint16_t temp = _wavetable[_index];
  _index = (_index +_freq) % 1024;
//  Serial.print(temp);
//  Serial.print(" ");
//  Serial.println(_index);
  //uint16_t temp2 = _wavetable[_index];

  return temp;
  };
void lfo::setFreq(uint16_t freq){
  _freq =freq;
  };
#endif
