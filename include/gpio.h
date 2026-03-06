#pragma once  
#include "base.h"
#include "common.h"
#define GPIO_BASE (MMIO_BASE + 0x200000)

// GPIO led
#define GPFSEL1   0x04
#define GPFSEL2   0x08  //sel1:0x04, sel2:0x08,..sel6:24=0x18
#define GPSET0    0x1C  //set0:28=16+12=0x1C
#define GPCLR0    0x28  //CLR0 = 28+4*3=40=32+8=0x28

// GPIO  used in uart

#define GPPUD     0x94
#define GPPUDCLK0 0x98
struct GpioPinData {
    reg32 reserved;
    reg32 data[2];
};
struct GpioRegs {
    reg32 func_select[6];//GPFSEL0 0x00, GPFSEL1 0x04, GPFSEL2: 0x08..., sel6:24=0x18
    struct GpioPinData output_set;//set0:28=16+12=0x1C
    struct GpioPinData output_clear;//CLR0 = 28+4*3=40=32+8=0x28
    struct GpioPinData level;
    struct GpioPinData ev_detect_status;
    struct GpioPinData re_detect_enable;
    struct GpioPinData fe_detect_enable;
    struct GpioPinData hi_detect_enable;
    struct GpioPinData lo_detect_enable;
    struct GpioPinData async_re_detect;
    struct GpioPinData async_fe_detect;
    reg32 reserved;
    reg32 pupd_enable;
    reg32 pupd_enable_clocks[2];
};
//
#define REGS_GPIO ((struct GpioRegs *)(MMIO_BASE + 0x00200000))

typedef enum _GpioFunc {
    GFInput = 0,
    GFOutput = 1,
    GFAlt0 = 4,
    GFAlt1 = 5,
    GFAlt2 = 6,
    GFAlt3 = 7,
    GFAlt4 = 3,
    GFAlt5 = 2
} GpioFunc;
void gpio_pin_set_func(uint8_t pinNumber, GpioFunc func);