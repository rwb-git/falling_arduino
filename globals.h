
#include <Arduino.h>

#define adaRGBPin 2

#define led_cnt 24  // number of leds in your strip. max value 255

#define loops_per_state 11    // we loop this many times for each led state. a loop is triggered for each timer 1 interrupt. so, if this value is small
                              // then the animation will be faster

extern uint8_t fade_length, rgb_flag, color, led_index, current_led_cnt;

extern int current_led;      // this will be both negative and positive

extern uint8_t *pixels;

extern uint8_t pix_data[];