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
struct fslp_data_t {
  uint16_t pressure;
  uint16_t pos;
};
class fslp {
  public:
    fslp(int fslpSenseLine, int fslpDriveLine1,
         int fslpDriveLine2, int fslpBotR0, int muxSel);
    ~fslp() {};

    fslp_data_t getData(); // Returns Pressure and Position for FSLP
    
  private:
    void clearLine();
    uint16_t pressureRead();
    uint16_t positionRead();
    
    int _fslpSenseLine;
    int _fslpDriveLine1;
    int _fslpDriveLine2;
    int _fslpBotR0;
    int _muxSel;
    uint8_t _state_count;
    fslp_data_t _FSLP_state [4]; // Stores last 4 readings

};
#endif

