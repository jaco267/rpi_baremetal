#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_SLAVE_NUM I2C_NUM_0
#define I2C_SLAVE_SDA 5
#define I2C_SLAVE_SCL 6
#define I2C_SLAVE_ADDR 0x08

void app_main(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_SLAVE,
        .sda_io_num = I2C_SLAVE_SDA,
        .scl_io_num = I2C_SLAVE_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,

        .slave.addr_10bit_en = 0,
        .slave.slave_addr = I2C_SLAVE_ADDR,
    };

    i2c_param_config(I2C_SLAVE_NUM, &conf);

    i2c_driver_install(
        I2C_SLAVE_NUM,
        conf.mode,
        128,
        128,
        0
    );

    printf("I2C slave ready\n");

    uint8_t data[32];

    while (1) {
        int len = i2c_slave_read_buffer(
            I2C_SLAVE_NUM,
            data,
            sizeof(data),
            100 / portTICK_PERIOD_MS
        );

        if (len > 0) {
            printf("Received: ");
            for (int i = 0; i < len; i++) {
                printf("%c", data[i]);   // 👉 印字母
            }
            printf("\n");
        }
    }
}