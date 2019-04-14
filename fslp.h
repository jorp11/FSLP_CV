#ifndef FLSP_H_
#define FSLP_H_
#include <stdint.h>
#include "Arduino.h"

enum FSLP_CHANNEL {
  FSLP_CHANNEL_1,
  FSLP_CHANNEL_2,
  FSLP_CHANNEL_3,
  FSLP_CHANNEL_4
};

class fslp {
  public:
    fslp(int fslpSenseLine, int fslpDriveLine1,
         int fslpDriveLine2, int fslpBotR0, int muxSel);
    ~fslp() {};
    int pressureRead();
    int positionRead();
    
  private:
      void clearLine();
    int _fslpSenseLine;
    int _fslpDriveLine1;
    int _fslpDriveLine2;
    int _fslpBotR0;
    int _muxSel;

};
#endif

