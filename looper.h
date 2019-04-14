#ifndef LOOPER_H_
#define LOOPER_H_
#include "Arduino.h"
#include "GPIO.h"
#define MAX_LOOP_SIZE 8192
enum FSLP_DATA_T {
  FSLP1_PRESS,
  FSLP1_POS,
  FSLP2_PRESS,
  FSLP2_POS
};

class looper {
  public:
    looper() {
      _counter = 0;
      _full = 0;
    };
    ~looper() {};
    void loopWrite(int press1, int pos1, int press2, int pos2) {
      // Serial.println(_counter);
      _loopArray[0][_counter] = press1;
      _loopArray[1][_counter] = pos1;
      _loopArray[2][_counter] = press2;
      _loopArray[3][_counter] = pos2;
      if (_counter == MAX_LOOP_SIZE - 1) {
        //_counter = 0;
        _full = 1;
      }
      else {
        _counter++;
        _full = 0;
      }
    };
    int getVal(FSLP_DATA_T val) {
      // Serial.printf("count_val: %i ",_counter);
      switch (val) {
        case FSLP1_PRESS: return _loopArray[0][_counter];
        case FSLP1_POS: return _loopArray[1][_counter];
        case FSLP2_PRESS: return _loopArray[2][_counter];
        case FSLP2_POS: return _loopArray[3][_counter];
      }
      //      if (_counter == LOOP_SIZE - 1)
      //        _counter = 0;
      //      else {
      //        _counter++;
      //      }
    }
    void incCounter() {
//      Serial.print(_counter);
//      Serial.print(" ");
//      Serial.println(_recLoopSize);
      _counter = (_counter + 1) % _recLoopSize;

    }
    bool isFull() {
      return _full;
    }
    void resetCounter() {
      _counter = 0;
    };
    void setRecSize() {
      _recLoopSize = _counter;
    }
  private:
    uint16_t _loopArray[4][MAX_LOOP_SIZE];
    int _counter;
    bool _full;
    int _recLoopSize = MAX_LOOP_SIZE;
};

#endif
