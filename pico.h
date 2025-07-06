#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#define LED   25
#define CS    13
#define RESET 23

void blink(duration){
for(int i=0;i<10;i++){gpio_put(25,0); sleep_ms(duration);
gpio_put(25,1); sleep_ms(duration);}}

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

}//end init_pico



