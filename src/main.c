#include <stdint.h>
#include "common.h"
#include "gpio.h"
#include "miniuart.h"
#include "utils.h"
#include "printf.h"
#include "irq.h"
#include "timer.h"
#include "i2c.h"

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

    gpio_pin_set_func(21,GFOutput); //*LED gpio21 //*done
    volatile uint32_t* gpio = (uint32_t*)GPIO_BASE;
    // 拉高 GPIO21 (led)
    gpio[GPSET0/4] = (1 << 21);
    //setup interrrupts
    irq_init_vectors();
    enable_interrupt_controller();
    irq_enable();
    
    timer_init();   
    printf("\nException Level: %d\n", get_el());
    printf("sleeping 1 seconds...\n");
    timer_sleep(1000);
    printf("Done!\n");
    
    printf("Initializing I2C...\n");
    i2c_init();
    // for (int i=0; i<10; i++) {
    //     char buffer[10];
    //     i2c_recv(21, buffer, 9);
    //     buffer[9] = 0;

    //     printf("Received: %s\n", buffer);

    //     timer_sleep(250);
    // }

    for (u8 d=0; d<20; d++) {
        i2c_send(21, &d, 1);
        timer_sleep(250);
        printf("Sent: %d\n", d);
    }

    char *msg = "Hello Slave Device";
    i2c_send(21, msg, 18);


    printf("DONE!\n");
    

    while(1) {
        // uart_send(uart_recv());  //這樣我們就可以打字了
    }
}
