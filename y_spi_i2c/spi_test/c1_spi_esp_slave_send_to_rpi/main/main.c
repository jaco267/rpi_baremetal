/*
https://www.youtube.com/watch?v=J-wr4fdvwBk&list=PLgrKXQgo8LPu_rr_19UcScZvbALLLJsjv&index=4
*/

// Slave as a transmitter for SPI communitation

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/igmp.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "soc/rtc_periph.h"
#include "driver/spi_slave.h"
#include "esp_log.h"
#include "spi_flash_mmap.h"
#include "driver/gpio.h"

// Pins in use
// #define GPIO_MOSI 12
// #define GPIO_MISO 13
// #define GPIO_SCLK 15
// #define GPIO_CS 14

#define GPIO_MOSI 7   //FSPID          
#define GPIO_MISO 2   //FSPIQ       
#define GPIO_SCLK 6   //FSPICLK       
#define GPIO_CS 10    //FSPICS0       


// Main application
void app_main(void){
    // Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    // Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=0,   //mode0 : CPOL = 0, CPHA = 0
        .spics_io_num=GPIO_CS, //SPI CS0       
        .queue_size=3,
        .flags=0,
    };
    // Initialize SPI slave interface
    spi_slave_initialize(SPI2_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    // SPI variables 
    uint8_t tx_data[] = {0xaa}; // 回覆給 rpi master 的訊息  10*16+10 = 170
    uint8_t rx_data[1];
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    while (1){
        memset(&t, 0, sizeof(t));
        t.length = 8;  //send 1 byte , 內容 0xAA
        t.tx_buffer = tx_data;
        t.rx_buffer = rx_data; 
        spi_slave_transmit(SPI2_HOST, &t, portMAX_DELAY);  //「卡住等 Master 發起 SPI 傳輸」
        //一旦 Master（你的 Python）送資料： ESP32 就同步回傳 tx_data
        printf("Master sent: %#x\n", rx_data[0]);
        printf("Slave  sent: %#x\n", tx_data[0]);
    }
}