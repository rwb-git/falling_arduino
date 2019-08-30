
#include "globals.h"

uint8_t brightness;

int minimum_virtual_led;

void save_one(uint8_t val){

  pix_data[led_index] = val;

  led_index++;
}

void store_color(void){

  uint8_t other = brightness >> 3; // instead of pure red, green, blue, add some white

  switch (color){

    case 0:

      save_one(brightness);   // green
      save_one(other);        // red
      save_one(other);        // blue

      break;

    case 1:

      save_one(other);
      save_one(brightness);
      save_one(other);

      break;

    case 2:

      save_one(other);
      save_one(other);
      save_one(brightness);

      break;
  }
}


void load_rgb(void){
  
  led_index = 0;

  brightness = 0;

  minimum_virtual_led = -255 / (int)(fade_length);  // setup() checks for fade_length < 1. virtual_leds are needed so that the fade tail runs completely off
                                                    // the end of the led strip connected to the nano. 
  
  for (int i = minimum_virtual_led; i<= led_cnt; i++){ 

    if (i == current_led){      // start illuminating now (but ignore virtual leds in call to store_color()). before this one, leds are all off
 
      brightness = 255;         // first led will be max brightness, and subsequent ones will be reduced

    } else {

      if (brightness >= fade_length){

        brightness -= fade_length;    // reduce brightness for leds after the first illuminated one

      } else {
   
        brightness = 0;        // we have finished fading, or we have not yet started illuminating
      }
    }

    if (i > 0){ // skip virtual leds
      store_color();
    }
  }

  if (current_led_cnt > loops_per_state){    // we loop this many times at each led. smaller numbers move faster

    current_led_cnt = 0;

    current_led--;      // the led closest to the nano is number 1, so, to make the animation "move" towards that end, we start at the other end
                        // and reduce current_led until it is at minumum_virtual_led and then we start over.

    if (current_led == (minimum_virtual_led - 1)){
      
      current_led = led_cnt; // start over at the far end of the strip.

      if (color < 2){     // one trace is red (or redish, if some white is added in store_color), then a blue trace, then green

        color++;
        
      } else {

        color = 0;
      }
    }
  } else {

    current_led_cnt++; 
  }
}


