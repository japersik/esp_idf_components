#ifndef JAPERSIK_ESP32_I2C_LCD_H
#define JAPERSIK_ESP32_I2C_LCD_H
#include "driver/i2c.h"

/**
 *  I2C slave device address
 */
typedef uint16_t i2c_address_t;

typedef struct {
    bool inited;
    i2c_port_t i2c_port;           // ESP-IDF I2C port number
    i2c_address_t address;         // I2C slave device address
    int timeout;                   // send timeout
} i2c_device_info_t;


esp_err_t i2c_device_init(i2c_device_info_t * i2c_device_info, i2c_port_t i2c_port, i2c_address_t address);
esp_err_t i2c_device_send_byte(const i2c_device_info_t * i2c_device_info, uint8_t data);

#endif //JAPERSIK_ESP32_I2C_LCD_H
