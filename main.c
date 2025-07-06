//main.c
#include <stdio.h>
#include "pico/stdlib.h"
#include "lora.h"

uint8_t payload[] = "hello from pico, clean library";

void main(){
stdio_init_all(); sleep_ms(3000);

  init_lora();
  sleep_ms(3000);
  for(;;){
    transmit(payload,sizeof(payload));
    sleep_ms(1500);
  }
  
}//end main
  
