//my_board.h
#ifndef MYBOARD_H
#define MYBOARD_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#define OPCODE_STDBY  0x80
//param 0 - STDBY_RC
#define SETSTDBY_RC (uint8_t[]){OPCODE_STDBY,0}

#define OPCODE_REGULATOR 0x96
//param 0x01 - USE_DCDC
#define SETREGULATOR (uint8_t[]){OPCODE_REGULATOR,0x01}

#define OPCODE_SETTXPARAMS 0x8E
//param 4 - RADIO_RAMP_200_US
#define SETTXPARAMS (uint8_t[]){OPCODE_SETTXPARAMS,0,4}

#define OPCODE_DIOIRQPARAMS 0x08
//params set dio1 interrupt
#define SETDIOIRQ (uint8_t[]){OPCODE_DIOIRQPARAMS,0x00,0x02,0xFF,0xFF,0x00,0x00,0x00,0x00}
//#define SETDIOIRQ (uint8_t[]){OPCODE_DIOIRQPARAMS,0xFF, 0xFF,0,0}


#define OPCODE_SETBUFFERADDRESS 0x8F
#define SETBUFFERADDRESS (uint8_t[]){OPCODE_SETBUFFERADDRESS,0,0}

#define OPCODE_SETSWITCHDIO2 0x9D
//param 1 - true
#define SETSWITCHDIO2 (uint8_t[]){OPCODE_SETSWITCHDIO2,1}

#define OPCODE_GETSTATUS 0xC0
#define GETSTATUS (uint8_t[]){OPCODE_GETSTATUS,0}

//params taken from waveshare testcode ping pong
#define OPCODE_MODULATION 0x8B
//param1 sf - 7 (SF7)
//param2 bw - 4
//param3 cr - 1
//param4 low data opt - 0
#define SETMODULATION (uint8_t[]){OPCODE_MODULATION,7,4,1,0}

//params taken from waveshare testcode ping pong
#define OPCODE_PACKETPARAMS 0x8C
//param1 - preamble len msb
//param2 - preamble len lsb
//param3 - header type
//param4 - payload length
//param5 - crc type
//param6 - invert iq
//#define SETPACKETPARAM (uint8_t[]){OPCODE_PACKETPARAMS,8,0,0,0xFF,1,0}
#define SETPACKETPARAM (uint8_t[]){OPCODE_PACKETPARAMS,0,8,0,0xFF,1,0}

#define OPCODE_PACKETTYPE 0x8A
//param 1 lora mode
#define SETPACKETTYPE (uint8_t[]){OPCODE_PACKETTYPE,1}

#define OPCODE_TXPARAM22 0x8E
//param 22 - power
//param 2 - ramp_40u
#define SETTXPARAMS22 (uint8_t[]){OPCODE_TXPARAM22, 22, 2}

//pa config taken from https://github.com/thekakester/Arduino-LoRa-Sx1262/blob/main/src/LoraSx1262.cpp

#define OPCODE_PACONFIG 0x95
//param1 duty cycle - 4
//param2 hpmax - 7
//param3 device select lora - 0
//reserve always 1
#define SETPACONFIG (uint8_t[]){OPCODE_PACONFIG,4,7,0,1}

//symbol timeout taken from https://github.com/thekakester/Arduino-LoRa-Sx1262/blob/main/src/LoraSx1262.cpp

#define OPCODE_SYMBOLTIMEOUT 0xA0
//param1 number of symbols
#define SETSYMBOLTIMEOUT (uint8_t[]){OPCODE_SYMBOLTIMEOUT,0}

#define OPCODE_SETTX 0x83
#define SETTX (uint8_t[]){OPCODE_SETTX,0xFF,0xFF,0xFF}

#define OPCODE_READREGISTER 0x1D

//read register 0x0740 result should be 0x14
#define READREGTEST (uint8_t[]){OPCODE_READREGISTER,0x07,0x40,0}

//set frequency to 915_000_000
#define OPCODE_SETFREQ915 0x86
#define SETFREQ915 (uint8_t[]){OPCODE_SETFREQ915,0x39,0x30,0,0}

#define OPCODE_CLEARIRQ 0x02
//param clear all interrupts
#define CLEARIRQ (uint8_t[]){OPCODE_CLEARIRQ,0xFF,0xFF}

#define OPCODE_RXBUFFERSTATUS 0x13
//param - radio status
//param - lorapacketlength
//param - memory offset
#define GETRXBUFFERSTATUS (uint8_t[]){OPCODE_RXBUFFERSTATUS,0xFF,0xFF,0xFF}

#define OPCODE_RXPACKETPARAM 0x8C
#define SETRXPACKETPARAM (uint8_t[]){OPCODE_RXPACKETPARAM,0x00,0x0C,0x00,0xFF,0x00,0x00}

#define OPCODE_SETRX 0x82
#define SETRX (uint8_t[]){OPCODE_SETRX,0xFF,0xFF,0xFF}

#define OPCODE_RXBUFFERSTATUS 0x13
//param1 - radio status
//param2 - lora packet length
//param3 - address memory offset
#define GETRXBUFFERSTATUS (uint8_t[]){OPCODE_RXBUFFERSTATUS,0xFF,0xFF,0xFF}

#endif
