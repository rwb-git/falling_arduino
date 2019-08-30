
#include "globals.h"

uint8_t   pix_data[led_cnt * 3];

uint8_t   *pixels = pix_data;    // pointer to the data that the adafruit code sends to the LEDs

uint8_t rgb_flag = 0;

uint8_t fade_length = 18;   // this value is subtracted from "brightness" for each led after the first one that is lit. so, the 
                            //number that are lit is 255 / fade_length. max value 255 = only one pixel lit, no fade tail. 
                            // minumum value looks good to me at 255 / led_cnt. you can use values as low as 1, but the leds will stay illuminated
                            // for a while as they slowly dim. 

uint8_t current_led_cnt = 0;

uint8_t color = 0;

uint8_t led_index = 0;

int current_led = led_cnt;
          