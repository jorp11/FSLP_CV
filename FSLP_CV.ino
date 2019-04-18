#include "fslp.h"
#include "GPIO.h"
#include "led.h"
#include "looper.h"
#include "buttons.h"
#include "ui.h"
#include "dac.h"
#include "mux.h"
#include "lfo.h"
#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1
#define LEDC_CHANNEL_2     2
#define LEDC_CHANNEL_3     3

const unsigned long READ_PERIOD = 4000; // SAMPLE period in uS
//Instantiate Drivers
fslp fslp1(SENSE_LINE_PIN, FSLP1_DRIVE_1_PIN, DRIVE_2_PIN, R0BOT_PIN, FSLP1_MUXSEL );
fslp fslp2(SENSE_LINE_PIN, FSLP1_DRIVE_1_PIN, DRIVE_2_PIN, R0BOT_PIN, FSLP2_MUXSEL );
led LED1(MODE1_LED_PIN, LEDC_CHANNEL_0  );
led LED2(MODE2_LED_PIN, LEDC_CHANNEL_1  );
led LED3(FSLP1_LED_PIN, LEDC_CHANNEL_2  );
led LED4(FSLP2_LED_PIN, LEDC_CHANNEL_3  );
//mux Mux;
ui UI;
looper Looper;
buttons Buttons;
dac Dac;
lfo Lfo1;
lfo Lfo2;
int buttonTimer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //delay(200);
  pinMode(MUX_SEL_A_PIN, OUTPUT);
  pinMode(MUX_SEL_B_PIN, OUTPUT);
  while (!Serial);

}

void loop() {
  static fslp_data_t fslp1Data;
  static fslp_data_t fslp2Data;
  static unsigned long lastRead;
  //If sample period has lapsed
  if (micros() - lastRead >= READ_PERIOD) {
    lastRead += READ_PERIOD;

    Buttons.debounce();
    UI_MODE curr_mode = UI.getMode();
    UI_MODE next_mode = curr_mode;

    if (curr_mode == UI_MODE_NORM) {
      //  Serial.println("NORM mode");
      fslp1Data = fslp1.getData();
      fslp2Data = fslp2.getData();

      if (Buttons.justPressed(0)) {
        next_mode = UI_MODE_LATCH;
        buttonTimer = millis();

      }
      else if (Buttons.justPressed(1)) {
        next_mode = UI_MODE_LOOP_REC;
        Looper.resetCounter();
      }
      else if (Buttons.justReleased(1) & Buttons.justReleased(0)) {
        next_mode = UI_MODE_LFO;

      }
    } else if (curr_mode == UI_MODE_LATCH) {
      //    Serial.println("Latch mode");
      if (Buttons.pressed(0) & (millis() - buttonTimer > 2000)) {
        next_mode = UI_MODE_LFO;
      }
      if (Buttons.justPressed(0)) {
        next_mode = UI_MODE_NORM;
      }
    } else if (curr_mode == UI_MODE_LOOP_REC) {
      fslp1Data = fslp1.getData();
      fslp2Data = fslp2.getData();

      Looper.loopWrite(fslp1Data.pressure, fslp1Data.pos, fslp2Data.pressure, fslp2Data.pos);

      if (Buttons.justPressed(1) || (Looper.isFull())) {
        //if(Buttons.justReleased(1)){Looper.setRecSize();}
        Looper.setRecSize();
        next_mode = UI_MODE_LOOP_PLAY;
        Looper.resetCounter();

      }
    }

    else if (curr_mode == UI_MODE_LOOP_PLAY) {
      if (Buttons.pressed(0)) {}
//Fake Latch
      else {
        fslp1Data.pressure = Looper.getVal(FSLP1_PRESS);
        fslp2Data.pressure = Looper.getVal(FSLP2_PRESS);
        fslp1Data.pos = Looper.getVal(FSLP1_POS);
        fslp2Data.pos = Looper.getVal(FSLP2_POS);
        Looper.incCounter();
        if (Buttons.justPressed(1)) {
          next_mode = UI_MODE_NORM;
        }
      }
    }
    else if (curr_mode == UI_MODE_LFO) {
     // fslp1Data = fslp1.getData();
      //fslp2Data = fslp2.getData();
      fslp_data_t temp = fslp1.getData();
    //  Serial.println(temp.pressure);
          //We're only throwing away 6 bits. Freqeuncy should be managed better to avoid this tbh

      Lfo1.setFreq((temp.pos >> 10) + 1); 
      fslp1Data.pressure = temp.pressure*Lfo1.getVal()>>16;
      fslp1Data.pos = temp.pos;
     // Serial.println(fslp1Data.pressure);
      temp = fslp2.getData();
      Lfo2.setFreq((temp.pos >> 10) + 1);
      fslp2Data.pressure = temp.pressure*Lfo2.getVal()>>16;
            fslp2Data.pos = temp.pos;


      if (Buttons.justPressed(0)) {
        next_mode = UI_MODE_NORM;
      }
    }

    LED3.ledcAnalogWrite(fslp1Data.pressure, 4096);
    LED4.ledcAnalogWrite(fslp2Data.pressure, 4096);
    if (curr_mode == UI_MODE_LATCH) {
      LED1.ledcAnalogWrite(0, 1024);
      LED2.ledcAnalogWrite(512, 1024);
    }
    else if (curr_mode == UI_MODE_LOOP_REC) {
      LED1.ledcAnalogWrite(512, 1024);
      LED2.ledcAnalogWrite(0, 1024);

    } else if (curr_mode == UI_MODE_LOOP_PLAY) {
      LED1.ledcAnalogWrite(512, 1024);
      LED2.ledcAnalogWrite(512, 1024);


    } else if (curr_mode == UI_MODE_LFO) {
      
        LED2.ledcAnalogWrite(fslp1Data.pressure, 1024);
        LED1.ledcAnalogWrite(fslp2Data.pressure, 1024);
    } else {
        LED2.ledcAnalogWrite(0, 1024);
        LED1.ledcAnalogWrite(0, 1024);
      }

      Dac.dacWriteChannels((fslp1Data.pressure), (fslp1Data.pos) , (fslp2Data.pressure ) , (fslp2Data.pos));

      UI.setMode(next_mode);
      //Serial.println(fslp1Data.pressure);
    }

  }
