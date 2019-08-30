#include <Arduino.h>
#include "globals.h"
#include "adafruit_16_MHz.h"
#include "rgb.h"


ISR(TIMER1_COMPA_vect){
    
  rgb_flag = 1;    
}



void init_timer_1_interrupt(void){

    TCCR1A = 0;
    TCCR1B = 0;     // Arduino or something is setting this register, because it is 0 on reset, but if this line is missing the timer does not work
                    // as intended

    TCCR1B = TCCR1B | ((1<<CS12) | (1<<CS10) | (1<< WGM12));

    OCR1A = 42;

    TIMSK1 = TIMSK1 | (1<<OCIE1A);

    TCNT1 = 0;
}



void setup(){

    noInterrupts();
  
    pinMode(adaRGBPin,OUTPUT);  

    init_timer_1_interrupt();

    interrupts(); 

    if (fade_length < 1){
        fade_length = 1; // avoid division by zero
    }
}




void loop(){

    if (rgb_flag == 1){

        rgb_flag = 0;

        load_rgb();

        adafruit();
    }
}
