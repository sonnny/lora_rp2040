//this is receive only program
//if you will be using as transceiver
//  call setModeReceive everytime you call lora_receive_async

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "lora.h"

uint8_t receiveBuff[255];

void main(){
stdio_init_all(); sleep_ms(3000);

init_lora();
sleep_ms(3000);
printf("receiving...\r\n");
setModeReceive();//just receive program for now, call it one time only

for(;;){
  int bytesRead = lora_receive_async(receiveBuff, sizeof(receiveBuff));
  if(bytesRead > -1) printf("%s\r\n",receiveBuff);
}}
