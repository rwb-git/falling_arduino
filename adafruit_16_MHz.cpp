#include <Arduino.h>
#include "globals.h"

void adafruit(){

    // this code is for 16 MHz devices, and is derived from adafruit's code at github for their neopixel products.

    volatile uint8_t next, *ptr, bit,hi,lo,p,b;
    volatile uint16_t i;

    i = led_cnt * 3;
    
    volatile uint8_t *port;         // Output PORT register
    uint8_t     pinMask;       // Output PORT bitmask
    
    p = adaRGBPin; 

    ptr = pixels,   // Pointer to next byte.
    
    b   = *ptr++,   // Current byte value
   
    port    = portOutputRegister(digitalPinToPort(p));
    pinMask = digitalPinToBitMask(p);

    hi   = *port |  pinMask;
    lo   = *port & ~pinMask;
    next = lo;
    bit  = 8;

      noInterrupts();           // disable all interrupts
    
    asm volatile(
     "head20:"                   "\n\t" // Clk  Pseudocode    (T =  0)
      "st   %a[port],  %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
      "sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
       "mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  4)
      "dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
      "st   %a[port],  %[next]"  "\n\t" // 2    PORT = next   (T =  7)
      "mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  8)
      "breq nextbyte20"          "\n\t" // 1-2  if(bit == 0) (from dec above)
      "rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
      "nop"                      "\n\t" // 1    nop           (T = 13)
      "st   %a[port],  %[lo]"    "\n\t" // 2    PORT = lo     (T = 15)
      "nop"                      "\n\t" // 1    nop           (T = 16)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
      "rjmp head20"              "\n\t" // 2    -> head20 (next bit out)
     "nextbyte20:"               "\n\t" //                    (T = 10)
      "ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 11)
      //"ldi  %[byte] ,  20" "\n\t" // 2    b = *ptr++    (T = 13)

      "ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 13)
      
      "st   %a[port], %[lo]"     "\n\t" // 2    PORT = lo     (T = 15)
      "nop"                      "\n\t" // 1    nop           (T = 16)
      "sbiw %[count], 1"         "\n\t" // 2    i--           (T = 18)
       "brne head20"             "\n"   // 2    if(i != 0) -> (next byte)

       // output section? port is clearly an input
       //
       // [symbolic name for code section]  "constraint"   (C expression)
      : [port]  "+e" (port),    // e means X Y or Z register pairs. port is porta, portb, etc. so, adafruit is using X Y or Z to contain the address of a port, so that the port can vary?
        [byte]  "+r" (b),       // r = any register, r0..r31
        [bit]   "+r" (bit),
        [next]  "+r" (next),
        [count] "+w" (i)        // w = the four upper register pairs, r24, X, Y, Z

        // input section
      : [ptr]    "e" (ptr),     // pointer to rgb byte array
        [hi]     "r" (hi),
        [lo]     "r" (lo));

        // no clobber section?
  interrupts();          

}


