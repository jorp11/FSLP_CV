#ifndef LOOPER_H_
#define LOOPER_H_
#include "Arduino.h"
#include "GPIO.h"
enum FSLP_DATA_T{
  FSLP1_PRESS,
  FSLP1_POS,
  FSLP2_PRESS,
  FSLP2_POS
};

class looper {
  public:
    looper(int dummy){
      _counter = 0;};
    ~looper() {};
    void loopWrite(int value){
      _loopArray[_counter] = value;
      _counter++;
      };
    int getVal(FSLP_DATA_T){
      
    }
  private:
    int _loopArray[8192];
    int _counter;
    

};

#endif
