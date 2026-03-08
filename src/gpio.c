#include "gpio.h"
#include "common.h"
/*
相當於 gpio_pin_set_func(21, GFOutput=1)
void gpio_init(){/// *this is replaced by gpio_pin_set_func
    volatile uint32_t* gpio = (uint32_t*)GPIO_BASE;
    /// 設 GPIO21 為 output           //uint32_t = 4*8 bits = 4 bytes 一格就是  4bytes
    uint32_t reg = gpio[GPFSEL2/4];      // 讀取寄存器 (每個寄存器 4 bytes)
    reg &= ~(7 << 3);                     // 清除 GPIO21 bits
    reg |= (1 << 3);                      // 設為 output
    gpio[GPFSEL2/4] = reg;
}
*/


void gpio_pin_set_func(uint8_t pinNumber, GpioFunc func){
    uint8_t bitStart = (pinNumber * 3) % 30; 
    uint8_t reg = pinNumber / 10; 
    uint32_t selector = REGS_GPIO->func_select[reg]; 
    //ex Tx pinNumber 14 select_reg = 14/10=1  bitStart 14*3 %30 = 42%30=12
    //ex Rx pinNumber 15 select_reg = 15/10=1  bitStart 15*3 %30 = 45%30=15
    //ex gpio 21         select_reg = 21/10=2  bitStart 21*3 %30 = 63%30=3
    selector &= ~(7<<bitStart);  //clearout 3 bits
    selector |= (func << bitStart); //ex func=GFOutput -> 1<<bitStart
    /*                                 if alt func select register 0  
     000=GPIO Pin input  把 func       Bits 27~29 GPIOpin9 start=3*9=27
     001=GPIO Pin output 推到 bit start     24~26 GPIOpin8 start=3*8=24
     100=GPIO Pin Alt0                       ...
     101=GPIO Pin Alt1                       0~3  GPIOpin0 
     110=GPIO Pin Alt2
     111=GPIO Pin Alt3
     011=GPIO Pin Alt4
     010=GPIO Pin Alt5
    */
    REGS_GPIO->func_select[reg] = selector; 
}