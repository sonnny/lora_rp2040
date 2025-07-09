//main.c
//todo add wait for completion and dont use delay 
#include <stdio.h>
#include "pico/stdlib.h"
#include "lora.h"

uint8_t ascii = 0x21;

//when changing payload length
//change config.h SETTXPACKETPARAM it's hardcoded
uint8_t payload[] = {0x41,0x42,0x43,0x44,0x45,0x46,0x00};//null terminated

void main(){
stdio_init_all(); sleep_ms(3000);
init_lora(); sleep_ms(3000);
printf("transmitting...\r\n");
for(;;){
  transmit(payload,sizeof(payload));
  if(ascii > 0x7D) ascii = 0x21;
  payload[5] = ascii++;
  sleep_ms(100);}}

  
