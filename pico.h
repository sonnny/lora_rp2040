#ifndef PICO_H
#define PICO_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/irq.h"
#include "pico/time.h"

#define LED     25
#define CS      13
#define RESET   23
#define DIO_1   16

/*

prototype of IrqHandler typedef void(IrqHandler)(unsigned int,long unsigned int);

void init_dio1(IrqHandler callback){
  gpio_init(DIO_1);
  gpio_set_dir(DIO_1,GPIO_IN);
  gpio_pull_down(DIO_1);
  gpio_set_irq_enabled_with_callback(
    DIO_1,
    GPIO_IRQ_EDGE_RISE,
    true,
    callback);}
*/

uint32_t millis(){
  to_ms_since_boot(get_absolute_time());}

void init_pico(){
//init spi
spi_init(spi1,5000*1000);
spi_set_format(spi1,8,SPI_CPOL_0,SPI_CPHA_0,SPI_MSB_FIRST);
gpio_set_function(14,GPIO_FUNC_SPI);
gpio_set_function(15,GPIO_FUNC_SPI);
gpio_set_function(24,GPIO_FUNC_SPI);

gpio_init(LED); gpio_set_dir(LED,GPIO_OUT); gpio_put(LED,0);//led
gpio_init(CS); gpio_set_dir(CS,GPIO_OUT); gpio_put(CS,1);
gpio_init(RESET); gpio_set_dir(RESET,GPIO_OUT); gpio_put(RESET,1);

gpio_init(DIO_1); gpio_set_dir(DIO_1,GPIO_IN);

}//end init_pico

#endif

