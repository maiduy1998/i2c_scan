#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <PCF8574.h>

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

#define SDA 21
#define SCL 22

#define DISABLE_SLAVE_RX_BUFFER_LEN 0
#define DISABLE_SLAVE_TX_BUFFER_LEN 0
#define INTR_ALLOC_FLAGS 0

/*
    The maximum clock frequency (fSCL (max)) is specified to be up to 400 kHz for I2C FM
    and up to 1000 kHz for FM+ spec.
*/
#define I2C_MASTER_FREQ_HZ 400000 // 400 kHz

#define ENABLE_ACK 0x01
#define SLAVE_ADDRESS 0x20
int i2c_master_port;


void i2c_init(int i2c_master_port)
{
    i2c_master_port = I2C_NUM_0;
    i2c_config_t i2c_config;
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = SDA;
    i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.scl_io_num = SCL;
    i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.master.clk_speed = I2C_MASTER_FREQ_HZ;
    // i2c_config.clk_flags = 0;

    esp_err_t err = i2c_param_config(i2c_master_port, &i2c_config);
    if (err != ESP_OK)
    {
        printf("Unable to setup I2C bus");
    }
    i2c_driver_install(i2c_master_port, i2c_config.mode, DISABLE_SLAVE_RX_BUFFER_LEN, DISABLE_SLAVE_TX_BUFFER_LEN, INTR_ALLOC_FLAGS);
}
void scan_device_i2C(void *ignore)
{
    i2c_init(i2c_master_port);

    while (1)
    {
        esp_err_t res;
        printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
        printf("00:         ");
        for (uint8_t i = 3; i < 0x78; i++)
        {
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
            i2c_master_stop(cmd);

            res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
            if (i % 16 == 0)
                printf("\n%.2x:", i);
            if (res == 0)
                printf(" %.2x", i);
            else
                printf(" --");
            i2c_cmd_link_delete(cmd);
        }
        printf("\n\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void i2c_write_byte(int slave_address, uint8_t value)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_address << 1) | I2C_MASTER_WRITE, ENABLE_ACK);
    i2c_master_write_byte(cmd, value, ENABLE_ACK);

    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

void i2c_master_send_data(void *pvParam){
    while(1){
        i2c_write_byte(SLAVE_ADDRESS, 0b00000011);//Write P0, turn off other pins
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}