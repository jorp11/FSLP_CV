#include "dac.h"
#include "Arduino.h"
#include "GPIO.h"
#include <SPI.h>

dac::dac() {
  pinMode(DAC_CS_PIN, OUTPUT);
  pinMode(DAC_RST_PIN, OUTPUT);
  digitalWrite(DAC_RST_PIN, HIGH);
  digitalWrite(DAC_CS_PIN, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE2));
}
void inline dac::dacWriteChannel(uint16_t value, DAC_CHANNEL_t channel) {
  uint8_t _DACout[3];
  digitalWrite(DAC_CS_PIN, LOW);
  switch (channel) {
    case DAC_CHANNEL_A :  _DACout[0] = B00010000; //write to channel A
    break;
    case DAC_CHANNEL_B :  _DACout[0] = B00010010; //writ to channel B
    break;
    case DAC_CHANNEL_C :  _DACout[0] = B00010100; //writ to channel C
    break;
    case DAC_CHANNEL_D :  _DACout[0] = B00010110; //writ to channel D
    break;
    default: Serial.println("Oops!");
  }
  _DACout[1] = uint8_t(value >> 8);
  _DACout[2] = uint8_t(value);
//  Serial.print(_DACout[0]);
//  Serial.print(" ");
//    Serial.print(_DACout[1]);
//      Serial.print(" ");
//
//  Serial.println(_DACout[2]);

  SPI.transfer(_DACout[0]);
  SPI.transfer(_DACout[1]);
  SPI.transfer(_DACout[2]);
  digitalWrite(DAC_CS_PIN, HIGH);
}
void  dac::dacWriteChannels(uint16_t valueA, uint16_t valueB,uint16_t valueC,uint16_t valueD   ){
  dacWriteChannel(valueA, DAC_CHANNEL_A);
  dacWriteChannel(valueB, DAC_CHANNEL_B);
  dacWriteChannel(valueC, DAC_CHANNEL_C);
  dacWriteChannel(valueD, DAC_CHANNEL_D);
}



