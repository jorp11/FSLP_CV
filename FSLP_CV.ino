#include "fslp.h"
#include "GPIO.h"
#include "led.h"
#include "looper.h"
#include "buttons.h"
#include "ui.h"
#include "dac.h"
#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1
//const int loop_led_pin     = 5;
const unsigned long READ_PERIOD = 1500;
fslp fslp1(SENSE_LINE_PIN, FSLP1_DRIVE_1_PIN, DRIVE_2_PIN, R0BOT_PIN, FSLP1_MUXSEL );
fslp fslp2(SENSE_LINE_PIN, FSLP1_DRIVE_1_PIN, DRIVE_2_PIN, R0BOT_PIN, FSLP2_MUXSEL );
led LED1(FSLP1_LED_PIN, LEDC_CHANNEL_0  );
led LED2(FSLP2_LED_PIN, LEDC_CHANNEL_1  );
ui UI;
looper Looper;
buttons Buttons;
dac Dac;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(200);
  pinMode(MUX_SEL_A_PIN, OUTPUT);
  pinMode(MUX_SEL_B_PIN, OUTPUT);
}

void loop() {

  static unsigned long lastRead;
  if (micros() - lastRead >= READ_PERIOD) {
    lastRead += READ_PERIOD;
    //static uint16_t fslp1_press, fslp2_press, fslp1_pos, fslp2_pos = 0;
    static fslp_data_t fslp1Data;
    static fslp_data_t fslp2Data;
    //static uint16_t fslp1_press_last, fslp2_press_last, fslp1_pos_last, fslp2_pos_last = 0;
    Buttons.debounce();
    UI_MODE curr_mode = UI.getMode();
    UI_MODE next_mode = curr_mode;

    if (curr_mode == UI_MODE_NORM) {
      //  Serial.println("NORM mode");
      fslp1Data = fslp1.getData();

      fslp2Data = fslp2.getData();
//      fslp1_press = fslp1.pressureRead();
//      fslp1_pos = fslp1.positionRead();
//      fslp2_press = fslp2.pressureRead();
//      fslp2_pos = fslp2.positionRead();
      if (Buttons.justPressed(0)) {
        //If we press the latch button, next mode is latch!
        next_mode = UI_MODE_LATCH;
      }
      else if (Buttons.justPressed(1)) {
        next_mode = UI_MODE_LOOP_REC;
              Looper.resetCounter();
      }


    } else if (curr_mode == UI_MODE_LATCH) {
      //    Serial.println("Latch mode");
 //     fslp1Data = fslp1.getData();

 //     fslp2Data = fslp1.getData();
//      fslp1_press = fslp1_press_last;
//      fslp1_pos = fslp1_pos_last;
//      fslp2_press = fslp2_press_last;
//      fslp2_pos = fslp2_pos_last;
      if (Buttons.justPressed(0)) {
        next_mode = UI_MODE_NORM;
      }
    } else if (curr_mode == UI_MODE_LOOP_REC) {
            fslp1Data = fslp1.getData();

      fslp2Data = fslp1.getData();
//      fslp1_press = fslp1.pressureRead();
//      fslp1_pos = fslp1.positionRead();
//      fslp2_press = fslp2.pressureRead();
//      fslp2_pos = fslp2.positionRead();
      //Looper.loopWrite(fslp1_press, fslp1_pos, fslp2_press, fslp2_pos);
        Looper.loopWrite(fslp1Data.pressure, fslp1Data.pos, fslp2Data.pressure, fslp2Data.pos);

      if (Buttons.justPressed(1)||(Looper.isFull())){
       //if(Buttons.justReleased(1)){Looper.setRecSize();}
        Looper.setRecSize();
        next_mode = UI_MODE_LOOP_PLAY;
        Looper.resetCounter();

      }
    }

    else if (curr_mode == UI_MODE_LOOP_PLAY) {
//          Serial.println("LOOP_PLAY mode");

      fslp1Data.pressure = Looper.getVal(FSLP1_PRESS);
      fslp2Data.pressure = Looper.getVal(FSLP2_PRESS);
      fslp1Data.pos = Looper.getVal(FSLP1_POS);
      fslp2Data.pos = Looper.getVal(FSLP2_POS);
      Looper.incCounter();
      if (Buttons.justPressed(1)) {
        next_mode = UI_MODE_NORM;
      }
    }


    // LED1.ledcAnalogWrite(fslp1_press, 1024);
    //     LED2.ledcAnalogWrite(fslp2_press, 1024);
    if (curr_mode == UI_MODE_LATCH) {
      LED1.ledcAnalogWrite(0, 1024);
      LED2.ledcAnalogWrite(1024, 1024);
    }
    else if (curr_mode == UI_MODE_LOOP_REC) {
      LED1.ledcAnalogWrite(1024, 1024);
      LED2.ledcAnalogWrite(0, 1024);

    } else if (curr_mode == UI_MODE_LOOP_PLAY) {
      LED1.ledcAnalogWrite(1024, 1024);
      LED2.ledcAnalogWrite(1024, 1024);

    }
    else {
      LED2.ledcAnalogWrite(0, 1024);
      LED1.ledcAnalogWrite(0, 1024);

    }
//    fslp1_press_last = fslp1_press;
//    fslp1_pos_last = fslp1_pos;
//    fslp2_press_last = fslp2_press;
//    fslp2_pos_last = fslp2_pos;

    Dac.dacWriteChannels(fslp1Data.pressure << 4, fslp1Data.pos << 4, fslp2Data.pressure << 4, fslp2Data.pos << 4);

    UI.setMode(next_mode);

  }
  else{
    }
}
