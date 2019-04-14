#ifndef LED_H_
#define LED_H_
#include "Arduino.h"
#include "GPIO.h"
#define LEDC_TIMER_13_BIT  13
#define LEDC_BASE_FREQ     5000

//enum LED_CHANNEL {
//  LED_CHANNEL_1,
//  LED_CHANNEL_2,
//  LED_CHANNEL_3,
//  LED_CHANNEL_4
//};

class led {
  public:
    led(int ledPin, int ledChannel);
    ~led() {};
    void ledcAnalogWrite(uint32_t value, uint32_t valueMax=255);
  private:
    int _ledPin;
    int _ledChannel;

};

#endif
