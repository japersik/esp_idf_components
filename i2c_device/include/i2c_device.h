#ifndef JAPERSIK_ESP_COMPONENTS_I2C_DEVICE_H
#define JAPERSIK_ESP_COMPONENTS_I2C_DEVICE_H
#include "driver/i2c.h"


//   I2C slave device address
typedef uint16_t i2c_address_t;

typedef struct {
    bool inited;
    i2c_port_t i2c_port;           // ESP-IDF I2C port number
    i2c_address_t address;         // I2C slave device address
    int timeout;                   // send timeout
} i2c_device_info_t;


esp_err_t i2c_device_init(i2c_device_info_t * i2c_device_info, i2c_port_t i2c_port, i2c_address_t address);

esp_err_t i2c_device_ping(const i2c_device_info_t * i2c_device_info);
esp_err_t i2c_device_send_byte(const i2c_device_info_t * i2c_device_info, uint8_t data);
esp_err_t i2c_device_receive_byte(const i2c_device_info_t * i2c_device_info, uint8_t * data);
esp_err_t i2c_device_send_bytes(const i2c_device_info_t * i2c_device_info, const uint8_t * data_pkg, size_t len);
esp_err_t i2c_device_receive_bytes(const i2c_device_info_t * i2c_device_info, uint8_t * data_pkg, size_t len);

#endif //JAPERSIK_ESP_COMPONENTS_I2C_DEVICE_H
