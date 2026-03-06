#include "gpio.h"
#include "miniuart.h"
#include "utils.h"
#include "aux.h"
void gpio_pin_enable(){
    volatile uint32_t* gpio = (uint32_t*)GPIO_BASE;
    gpio[GPPUD/4] = 0;
    delay(150);
    gpio[GPPUDCLK0/4] = (1 << 14) | (1 << 15);
    delay(150);
    gpio[GPPUDCLK0/4] = 0;
}
void uart_init(void){
    // volatile uint32_t* aux  = (uint32_t*)AUX_BASE;
    REGS_AUX->enables = 1; // 1. enable mini uart
    REGS_AUX->mu_control = 0;  // 2. disable tx/rx during config
    REGS_AUX->mu_lcr = 3;      // 3. 8-bit mode
    REGS_AUX->mu_ier = 0xD;      // 4. interrupt
    REGS_AUX->mu_baud_rate = 270; // 5. baud rate (115200)
    // 6. 設 GPIO14,15 為 ALT5 (UART1)
    gpio_pin_set_func(14, GFAlt5); //GFAlt5 = 2
    gpio_pin_set_func(15, GFAlt5);
    gpio_pin_enable(); // 7. disable pull-up/down
    REGS_AUX->mu_control = 3;  // 8. enable tx/rx
}
void uart_send(char c){
    volatile uint32_t* aux = (uint32_t*)AUX_BASE;
    while (!(aux[AUX_MU_LSR_REG/4] & 0x20));
    aux[AUX_MU_IO_REG/4] = c;
}
void uart_send_string(const char* s){  
    while (*s) {
        if (*s == '\n')
            uart_send('\r');
        uart_send(*s++);
    }
}

/*p15 data ready*/
char uart_recv(){  //haven't not test yet
    while(!(REGS_AUX->mu_lsr & 1));
    return REGS_AUX->mu_io & 0xFF;
}
