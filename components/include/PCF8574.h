// #define PCF8574_IS_HYBRID

void i2c_init(int i2c_master_port);
/**
 * @brief
 * @param  i2c_master_port
 */

void scan_device_i2C(void *ignore);
/**
 * @brief
 * @param  ignore
 */
void i2c_write_byte(int slave_address, uint8_t value);
/**
 * @brief
 * @param  slave_address
 * @param  value
 */
void i2c_master_send_data(void *pvParam);
/**
 * @brief
 * @param  pvParam
 */
