#pragma once 
#define AUX_BASE  (MMIO_BASE + 0x215000)

// mini UART registers
#define AUX_ENABLES     0x04
#define AUX_MU_IO_REG   0x40
#define AUX_MU_IER_REG  0x44
#define AUX_MU_LCR_REG  0x4C
#define AUX_MU_LSR_REG  0x54
#define AUX_MU_CNTL_REG 0x60
#define AUX_MU_BAUD_REG 0x68
void uart_init(void);
void uart_send(char c);
void uart_send_string(const char* s);
char uart_recv();