//modified from https://github.com/thekakester/Arduino-LoRa-Sx1262/blob/main/src/LoraSx1262.cpp
//see config.h for opcode and params

#ifndef LORA_H
#define LORA_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pico.h"
#include "config.h"

//static uint8_t fires = 0;

uint8_t spiBuffer[32];

//void dio1_callback(uint gpio, uint32_t events){
//fires = 1; }
//gpio_acknowledge_irq(gpio,events);}
 //printf("dio1 irq events: %X\r\n",events);
 // gpio_acknowledge_irq(gpio,events);}

/***********************
 * lora init function
 **********************/
void init_lora(){

  init_pico();
  //init_dio1(dio1_callback);//interrupt pin
  
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

/*
bool waitForRadioCommandCompletion(uint32_t timeout){
printf("waitfor radio completion\r\n");
  uint32_t startTime = millis();
  bool dataTransmitted = false;
  while(!dataTransmitted){
    sleep_ms(5);
    spiBuffer[0] = 0xC0; //getstatus
    spiBuffer[1] = 0x00;
    gpio_put(CS,0);
    spi_write_read_blocking(spi1,spiBuffer,bufferResult,2);
    gpio_put(CS,1);
    
    printf("wait for radio completion result %X\r\n",bufferResult[1]);
    
    uint8_t temp = bufferResult[1];
    uint8_t chipMode = (bufferResult[1] >> 4) & 0x07;
    uint8_t commandStatus = (bufferResult[1] >> 1) & 0x07;
    
    if (commandStatus != 0 &&
        commandStatus != 1 &&
        commandStatus != 2) {dataTransmitted = true;}
     
    //if were on standby mode, no nedd to wait at all
    //0x03 - STBY_XOSC, 0x02 = STBY_RC   
    if (chipMode == 0x03 || chipMode == 0x02){dataTransmitted = true;}
    
    if (millis() - startTime >= timeout) {
      return false;}
  }//while
      
  return true;}
  
  */
    

/********************
 * transmit function
 ********************/
void transmit(uint8_t *data, int dataLen){
  //standby
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETSTDBY_RC,sizeof(SETSTDBY_RC));
  gpio_put(CS,1); sleep_ms(100);
  
  //setpacketparam
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETTXPACKETPARAM,sizeof(SETTXPACKETPARAM));
  gpio_put(CS,1);
  sleep_ms(100);
  //waitForRadioCommandCompletion(100);
  
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
    spi_write_blocking(spi1,spiBuffer,size);}//write payload
  gpio_put(CS,1);
  sleep_ms(300);
  //waitForRadioCommandCompletion(1000);
  
  //settx actual transmit
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETTX,sizeof(SETTX));
  gpio_put(CS,1);
  sleep_ms(300);
  //waitForRadioCommandCompletion(12000);
  
}//end transmit

void setModeReceive(){
//printf("setrxpacketparam\r\n");
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETRXPACKETPARAM,sizeof(SETRXPACKETPARAM));
  gpio_put(CS,1);
  sleep_ms(100);
  //waitForRadioCommandCompletion(100);
 
 //printf("setrx\r\n");
  gpio_put(CS,0);
  spi_write_blocking(spi1,SETRX,sizeof(SETRX));
  gpio_put(CS,1);
  sleep_ms(100);
  //waitForRadioCommandCompletion(100);
} 
/*********************
 * received function
 ********************/
int lora_receive_async(uint8_t* buff, int buffMaxLen){

  uint8_t bufferResult[32];

  //setModeReceive();

//printf("get dio_1\r\n");
  //dio_1 = 0 no packet received
  //dio_1 = 1 packet is ready
  if(gpio_get(DIO_1) == 0) {return -1;}
  
//printf("clear irq status\r\n");
  //clearirqstatus
  gpio_put(CS,0);
  spi_write_blocking(spi1,CLEARIRQ,sizeof(CLEARIRQ));
  gpio_put(CS,1);
  sleep_ms(100);
  
//printf("getrxbufferstatus\r\n");
  //getrxbufferstatus
  //write GETRXBUFFERSTATUS
  //read result to bufferResult
  gpio_put(CS,0);             
  spi_write_read_blocking(spi1,GETRXBUFFERSTATUS,bufferResult,sizeof(GETRXBUFFERSTATUS));
  gpio_put(CS,1); sleep_ms(100);
  
  uint8_t payloadLen   = bufferResult[2];   //how long the lora packet is
  uint8_t startAddress = bufferResult[3];   //where in memory is the packet
  
  if(buffMaxLen < payloadLen) {payloadLen = buffMaxLen;}
  
  //printf("read buffer\r\n");
  //read buffer
  spiBuffer[0] = 0x1E; //opcode read buffer
  spiBuffer[1] = startAddress;
  spiBuffer[2] = 0x00; //dummy byte
  
  //copy received data to buff
  //buff is uint8_t pointer from main
  //pass as a param
  gpio_put(CS,0);
  spi_write_blocking(spi1,spiBuffer,3);
  spi_write_read_blocking(spi1,spiBuffer,buff,payloadLen);
  gpio_put(CS,1);
  return payloadLen;
}//end lora_receive_async

#endif
