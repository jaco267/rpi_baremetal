#include <stdint.h>
#include "common.h"
#include "gpio.h"
#include "miniuart.h"
#include "utils.h"
#include "printf.h"
#include "irq.h"
#include "timer.h"
#include "i2c.h"
#include "spi.h"
#include "mailbox.h"
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
void run_i2c(){
    printf("Initializing I2C...\n");
    i2c_init();
    /// for (int i=0; i<10; i++) {
    ///     char buffer[10];
    ///     i2c_recv(21, buffer, 9);
    ///     buffer[9] = 0;
    ///     printf("Received: %s\n", buffer);
    ///     timer_sleep(250);
    /// }
    for (u8 d=0; d<20; d++) {
        i2c_send(21, &d, 1);
        timer_sleep(250);
        printf("Sent: %d\n", d);
    }
    char *msg = "Hello Slave Device";
    i2c_send(21, msg, 18);
}
void run_spi(){
    volatile uint32_t* gpio = (uint32_t*)GPIO_BASE;
    printf("Initializing SPI...\n"); 
    spi_init();
    // u8 cmd[2]; 
    // cmd[0] = 3;
    // cmd[1] = 1; 
    // for(int i =0; i<=9; i++){
    //     spi_send(0,cmd, 2); 
    //     timer_sleep(200); 
    // }
    u8 tx_cmd[1]; tx_cmd[0] = 42;
    u8 rx_cmd[1]; rx_cmd[0] = 0;  
    for(int i =0; i<=30; i++){
        gpio[GPCLR0/4] = (1 << 8);   // CS ↓（開始）
        timer_sleep_mus(5);  //  很重要（ESP32需要）
        // spi_send(0,tx_cmd, 1); 
        spi_send_recv(0, tx_cmd, rx_cmd, 1);
        timer_sleep_mus(5);       // 等最後 clock
        gpio[GPSET0/4] = (1 << 8);// CS ↑
        timer_sleep(500);
        printf("send: %d\n", tx_cmd[0]);
        printf("receive: %d\n", rx_cmd[0]); 
    }
    timer_sleep(2000); 
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
    
    //run_i2c()
    //run_spi()
    
    printf("MAILBOX:\n"); 

    printf("CORE CLOCK: %d\n", mailbox_clock_rate(CT_CORE));
    printf("EMMC CLOCK: %d\n", mailbox_clock_rate(CT_EMMC));
    printf("UART CLOCK: %d\n", mailbox_clock_rate(CT_UART));
    printf("ARM  CLOCK: %d\n", mailbox_clock_rate(CT_ARM));

    printf("I2C POWER STATE:\n");

    for (int i=0; i<3; i++) {
        bool on = mailbox_power_check(i);

        printf("POWER DOMAIN STATUS FOR %d = %d\n", i, on);
    }

    //timer_sleep(2000);

    for (int i=0; i<3; i++) {
        u32 on = 1;
        mailbox_generic_command(RPI_FIRMWARE_SET_DOMAIN_STATE, i, &on);

        printf("SET POWER DOMAIN STATUS FOR %d = %d\n", i, on);
    }

    //timer_sleep(1000);

    for (int i=0; i<3; i++) {
        bool on = mailbox_power_check(i);

        printf("POWER DOMAIN STATUS FOR %d = %d\n", i, on);
    }

    u32 max_temp = 0;

    mailbox_generic_command(RPI_FIRMWARE_GET_MAX_TEMPERATURE, 0, &max_temp);


    printf("DONE!\n");
    

    while(1) {
        // uart_send(uart_recv());  //這樣我們就可以打字了
    }
}
