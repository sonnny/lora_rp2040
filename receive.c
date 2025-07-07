#include <stdio.h>
#include "pico/stdlib.h"
#include "lora.h"

uint8_t receiveBuff[255];

void main(){
stdio_init_all(); sleep_ms(3000);

init_lora();
sleep_ms(3000);

for(;;){
  int bytesRead = lora_receive_async(receiveBuff, sizeof(receiveBuff));
  if(bytesRead > -1) printf("%s\r\n",receiveBuff);
}}
