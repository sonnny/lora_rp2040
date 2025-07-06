//init from https://github.com/thekakester/Arduino-LoRa-Sx1262/blob/main/src/LoraSx1262.cpp
#include <string.h>
#include "pico.h"
#include "config.h"

uint8_t spiBuffer[32];

void init_lora(){
printf("init lora\n");
  init_pico();
  //reset
  gpio_put(RESET,0); sleep_ms(100);
  gpio_put(RESET,1); sleep_ms(100);
  gpio_put(RESET,0); sleep_ms(100);
  gpio_put(RESET,1); sleep_ms(100);
  
//SetDIO2AsRfSwitchCtrl
gpio_put(CS,0);
spi_write_blocking(spi1,SETSWITCHDIO2,sizeof(SETSWITCHDIO2));
gpio_put(CS,1); sleep_ms(100);

//setfrequency 915mHz
gpio_put(CS,0);
spi_write_blocking(spi1,SETFREQ915,sizeof(SETFREQ915));
gpio_put(CS,1); sleep_ms(100);

//setpackettype
gpio_put(CS,0);
spi_write_blocking(spi1,SETPACKETTYPE,sizeof(SETPACKETTYPE));
gpio_put(CS,1); sleep_ms(100);

//setmodulation
gpio_put(CS,0);
spi_write_blocking(spi1,SETMODULATION,sizeof(SETMODULATION));
gpio_put(CS,1); sleep_ms(100);

//setpaconfig
gpio_put(CS,0);
spi_write_blocking(spi1,SETPACONFIG,sizeof(SETPACONFIG));
gpio_put(CS,1); sleep_ms(100);

//settxparams
gpio_put(CS,0);
spi_write_blocking(spi1,SETTXPARAMS22,sizeof(SETTXPARAMS22));
gpio_put(CS,1); sleep_ms(100);

//setlorasymbnumtimeout
gpio_put(CS,0);
spi_write_blocking(spi1,SETSYMBOLTIMEOUT, sizeof(SETSYMBOLTIMEOUT));
gpio_put(CS,1); sleep_ms(100);

//setdioirqparams
gpio_put(CS,0);
spi_write_blocking(spi1,SETDIOIRQ, sizeof(SETDIOIRQ));
gpio_put(CS,1); sleep_ms(100);

}//end init_lora

void transmit(uint8_t *data, int dataLen){
  //standby
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETSTDBY_RC,sizeof(SETSTDBY_RC));
  gpio_put(CS,1); sleep_ms(100);
  
  //setpacketparam -- from pingpong could be problem
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETPACKETPARAM,sizeof(SETPACKETPARAM));
  gpio_put(CS,1); sleep_ms(100);
  
  //write buffer
  spiBuffer[0] = 0x0E;
  spiBuffer[1] = 0x00;
  gpio_put(CS,0);
  spi_write_blocking(spi1,spiBuffer,2);
  
  //write rest of data
  uint8_t size = sizeof(spiBuffer);
  for(uint16_t i=0; i<dataLen; i += size){
    if (i + size > dataLen) { size = dataLen - i; }
    memcpy(spiBuffer,&(data[i]),size);
    spi_write_blocking(spi1,spiBuffer,size);}
  gpio_put(CS,1); sleep_ms(1000);
  
  //settx actual transmit
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETTX,sizeof(SETTX));
  gpio_put(CS,1); sleep_ms(1000);
  
}//end transmit
  
void register_test(){
uint8_t result;
uint8_t dummy=0;
gpio_put(CS,0);
spi_write_blocking(spi1,READREGTEST,sizeof(READREGTEST));
int status=spi_write_read_blocking(spi1,&dummy,&result,1);
gpio_put(CS,1);
if(result == 0x14) blink(1000);
else blink(100);}

