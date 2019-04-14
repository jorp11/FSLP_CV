#include "Arduino.h"
#include "led.h"
#include "GPIO.h"

led::led(int ledPin, int ledChannel)
{
  _ledPin = ledPin;
  _ledChannel = ledChannel;
    pinMode(ledPin, OUTPUT);
  ledcSetup(ledChannel, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(ledPin, ledChannel);
}

void led::ledcAnalogWrite(uint32_t value, uint32_t valueMax) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  // write duty to LEDC
  ledcWrite(_ledChannel, duty);
}
