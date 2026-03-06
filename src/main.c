#include <stdint.h>
#include "common.h"
#include "gpio.h"
#include "miniuart.h"
#include "utils.h"
#include "printf.h"
#include "irq.h"
#include "timer.h"
void putc(void *p, char c) {
    if (c == '\n') {
        uart_send('\r');
    }
    uart_send(c);
}
uint32_t get_el(void){
    uint32_t el;
    asm volatile (//mrs:move register from system register
        "mrs %0, CurrentEL\n"       //mrs x0, CurrentEL (exception level)
        "lsr %0, %0, #2\n"          //lsr x0,x0,#2  //right shift x0 #2
        : "=r" (el)                 //ret
    );

    return el;
}
// 入口函數
void main(void) {
    uart_init();
    init_printf(0, putc);

    printf("\nUART OK!\n");
    printf("\nRasperry PI 3B+ Bare Metal OS Initializing gogo...\n");

    gpio_pin_set_func(21,GFOutput); //*LED gpio21
    volatile uint32_t* gpio = (uint32_t*)GPIO_BASE;
    // 拉高 GPIO21 (led)
    gpio[GPSET0/4] = (1 << 21);

    
    //setup interrrupts
    irq_init_vectors();
    enable_interrupt_controller();
    irq_enable();
    
    timer_init();   
    printf("\nException Level: %d\n", get_el());
    printf("sleeping 200 ms...\n");
    timer_sleep(200);
    printf("sleeping 200 ms...\n");
    timer_sleep(200);
    printf("sleeping 200 ms...\n");
    timer_sleep(200);
    printf("sleeping 2 seconds...\n");
    timer_sleep(2000);

    printf("sleeping 2 seconds...\n");
    timer_sleep(2000);

    printf("sleeping 5 seconds...\n");
    timer_sleep(5000);
    printf("Done!\n");
    while(1) {
        // uart_send(uart_recv());  //這樣我們就可以打字了
    }
}
