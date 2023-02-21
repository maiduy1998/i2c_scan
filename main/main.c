#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#include "PCF8574.h"

#define SDA 21
#define SCL 22

#define DISABLE_SLAVE_RX_BUFFER_LEN 0
#define DISABLE_SLAVE_TX_BUFFER_LEN 0
#define INTR_ALLOC_FLAGS            0

/*
    The maximum clock frequency (fSCL (max)) is specified to be up to 400 kHz for I2C FM 
    and up to 1000 kHz for FM+ spec.
*/
#define I2C_MASTER_FREQ_HZ 400000 //400 kHz

#define ENABLE_ACK           0x01
#define SLAVE_ADDRESS       0x20

int  i2c_master_port;

void app_main(void)
{
    // i2c_init(i2c_master_port);
    
    xTaskCreate(scan_device_i2C, "I2C master sends data", 2048, NULL, 1, NULL);
}