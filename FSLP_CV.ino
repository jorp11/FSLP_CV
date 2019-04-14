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

fslp fslp1(SENSE_LINE_PIN, FSLP1_DRIVE_1_PIN, DRIVE_2_PIN, R0BOT_PIN, FSLP1_MUXSEL );
fslp fslp2(SENSE_LINE_PIN, FSLP1_DRIVE_1_PIN, DRIVE_2_PIN, R0BOT_PIN, FSLP2_MUXSEL );
led LED1(FSLP1_LED_PIN, LEDC_CHANNEL_0  );
led LED2(FSLP2_LED_PIN, LEDC_CHANNEL_1  );
ui UI;
looper Looper(5);
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
  static int fslp1_press, fslp2_press, fslp1_pos, fslp2_pos = 0;
  static int count= 2^5;
  static int fslp1_press_last, fslp2_press_last, fslp1_pos_last, fslp2_pos_last = 0;
  Buttons.debounce();
  if (Buttons.pressed(0)) {
    UI.setMode(UI_MODE_LATCH);
  }
  else if (Buttons.pressed(1)) {
    UI.setMode(UI_MODE_LOOP_REC);
  }
  UI_MODE curr_mode = UI.getMode();

  if ((curr_mode == UI_MODE_NORM) || (curr_mode == UI_MODE_LOOP_REC)) {
    fslp1_press = fslp1.pressureRead();
    fslp1_pos = fslp1.positionRead();
    fslp2_press = fslp2.pressureRead();
    fslp2_pos = fslp2.positionRead();
  }
  else if (curr_mode == UI_MODE_LOOP_PLAY) {
    fslp1_press = Looper.getVal(FSLP1_PRESS);
  }
  else if (curr_mode == UI_MODE_LATCH) {
    fslp1_press = fslp1_press_last;
    fslp1_pos = fslp1_pos_last;
    fslp2_press = fslp2_press_last;
    fslp2_pos = fslp2_pos_last;

  }

  LED1.ledcAnalogWrite(fslp1_press, 1024);
  LED2.ledcAnalogWrite(fslp2_press, 1024);
  fslp1_press_last = fslp1_press;
  fslp1_pos_last = fslp1_pos;
  fslp2_press_last = fslp2_press;
  fslp2_pos_last = fslp2_pos;
  //  Serial.print(fslp1_press);
  //  Serial.print(" ");
  //  Serial.print(fslp1_pos);
  //  Serial.print(" ");
  //  Serial.print(fslp2_press);
  //  Serial.print(" ");
  //  Serial.println(fslp2_pos);

  Dac.dacWrite(fslp1_press<<4, DAC_CHANNEL_A);
  Dac.dacWrite(fslp1_pos<<4, DAC_CHANNEL_B);
  Dac.dacWrite(fslp2_press<<4, DAC_CHANNEL_C);
  Dac.dacWrite(fslp2_pos<<4, DAC_CHANNEL_D);

  //Serial.println(micros());
  delay(10);
}
