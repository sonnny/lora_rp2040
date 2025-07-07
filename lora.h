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

//stoptimeronpreamble
spiBuffer[0] = 0x9F;
spiBuffer[1] = 0x00;
gpio_put(CS,0);
spi_write_blocking(spi1,spiBuffer,2);
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

void setModeReceive(){
  //setpacketparam
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETRXPACKETPARAM,sizeof(SETRXPACKETPARAM));
  gpio_put(CS,1); sleep_ms(100);
  
  //setrx
  spiBuffer[0] = 0x82;
  spiBuffer[1] = 0xFF;
  spiBuffer[2] = 0xFF;
  spiBuffer[3] = 0xFF;
  gpio_put(CS,0);
  spi_write_blocking(spi1,spiBuffer,4);
  gpio_put(CS,1); sleep_ms(100);}
  
int lora_receive_async(uint8_t* buff, int buffMaxLen){

uint8_t bufferResult[32];

//printf("lora_receive_async \r\n");

  setModeReceive();
  if(gpio_get(DIO_1) == 0) {return -1;} //when high packet is ready
  
  //clearirqstatus
  gpio_put(CS,0);
  spi_write_blocking(spi1,CLEARIRQ,sizeof(CLEARIRQ));
  gpio_put(CS,1); sleep_ms(100);
  
  //getrxbufferstatus
  spiBuffer[0] = 0x13;
  spiBuffer[1] = 0xFF;
  spiBuffer[2] = 0xFF;
  spiBuffer[3] = 0xFF;
  gpio_put(CS,0);
  spi_write_read_blocking(spi1,spiBuffer,bufferResult,4);
  gpio_put(CS,1); sleep_ms(100);
  
  uint8_t payloadLen =   bufferResult[2];   //how long the lora packet is
  uint8_t startAddress = bufferResult[3];   //where in memory is the packet
  
  if(buffMaxLen < payloadLen) {payloadLen = buffMaxLen;}
  
  //read buffer
  spiBuffer[0] = 0x1E; //opcode read buffer
  spiBuffer[1] = startAddress;
  spiBuffer[2] = 0x00; //dummy byte
  
  gpio_put(CS,0);
  spi_write_blocking(spi1,spiBuffer,3);
  spi_write_read_blocking(spi1,spiBuffer,buff,payloadLen);
  gpio_put(CS,1);
  return payloadLen;
}//end lora_receive_async
