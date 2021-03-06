#include "Arduino.h"
#include "fslp.h"
#include "GPIO.h"

fslp::fslp(int fslpSenseLine, int fslpDriveLine1,
           int fslpDriveLine2, int fslpBotR0, int muxSel)
{
  _fslpSenseLine = fslpSenseLine;
  _fslpDriveLine1 = fslpDriveLine1;
  _fslpDriveLine2 = fslpDriveLine2;
  _fslpBotR0 = fslpBotR0;
  _muxSel = muxSel & 0x3; // 0-3. drives multiplexer select line.
  _state_count = 0;
}
uint16_t fslp::pressureRead() {

  digitalWrite(MUX_SEL_B_PIN, _muxSel >> 1 );
  digitalWrite(MUX_SEL_A_PIN, _muxSel & 0x1 );
  //Serial.print("mux line: ");
  //Serial.println(_muxSel);
  delayMicroseconds(5);

  clearLine();

  // Step 2 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 3 - Take two measurements.
  int v1 = analogRead(_fslpDriveLine2);
  int v2 = analogRead(_fslpSenseLine);

  // Step 4 - Calculate the pressure.
  if (v1 == v2)
  {
    // Avoid dividing by zero, and return maximum reading.
    return 4095;//2 << 12 - 1; //4*64 * 1023;
  }
  return 12 * 64 * v2 / (v1 - v2);
}

uint16_t fslp::positionRead()
{
  // Step 1 - Clear the charge on the sensor.
  pinMode(_fslpSenseLine, OUTPUT);
  digitalWrite(_fslpSenseLine, LOW);

  pinMode(_fslpDriveLine1, OUTPUT);
  digitalWrite(_fslpDriveLine1, LOW);

  pinMode(_fslpDriveLine2, OUTPUT);
  digitalWrite(_fslpDriveLine2, LOW);

  pinMode(_fslpBotR0, OUTPUT);
  digitalWrite(_fslpBotR0, LOW);

  // Step 2 - Set up appropriate drive line voltages.
  digitalWrite(_fslpDriveLine1, HIGH);
  pinMode(_fslpBotR0, INPUT);
  pinMode(_fslpSenseLine, INPUT);

  // Step 3 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 4 - Take the measurement.
  return analogRead(_fslpSenseLine);
}
void fslp::clearLine()
{
  pinMode(_fslpDriveLine1, OUTPUT);
  digitalWrite(_fslpDriveLine1, HIGH);

  pinMode(_fslpBotR0, OUTPUT);
  digitalWrite(_fslpBotR0, LOW);

  pinMode(_fslpSenseLine, INPUT);

  pinMode(_fslpDriveLine2, INPUT);

}
fslp_data_t fslp::getData() {
  fslp_data_t temp, fslpData{0, 0};
  temp.pressure = pressureRead();
  temp.pos = positionRead();
  _FSLP_state[_state_count] = temp;
  //Average the stuff. TODO replace magic number(4/state_count) withconstant
  for (int i = 0; i != 4; i++) {
    fslpData.pressure += _FSLP_state[i].pressure;
    fslpData.pos += _FSLP_state[i].pos;
  }
  fslpData.pressure = fslpData.pressure << 2; //Divide by 4, shift by 4 for DAC
  fslpData.pos = fslpData.pos << 2;
  _state_count = (_state_count + 1) & 0x3;
  return fslpData;
};


