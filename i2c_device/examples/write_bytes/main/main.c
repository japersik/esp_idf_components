#include "main.h"

static const char *TAG = "demo_write_byte";

#define SDA_PORT        GPIO_NUM_21
#define SCL_PORT        GPIO_NUM_22
#define I2C_PORT        I2C_NUM_0
#define I2C_FREQUENCY   400000

esp_err_t i2c_init(void)
{
    esp_err_t ret;
    i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_PORT,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_io_num = SCL_PORT,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = I2C_FREQUENCY
    };
    ret = i2c_param_config(I2C_PORT, &conf);
    if (ret != ESP_OK) return ret;
    ret = i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
    return ret;
}

void app_main(void){
    ESP_LOGI(TAG, "Write bytes example");
    i2c_init();
    i2c_device_info_t i2c_device_info;
    i2c_device_init(&i2c_device_info,I2C_PORT,0x27);
    i2c_device_send_byte(&i2c_device_info,0b10101010);
    ESP_LOGI(TAG, "Done");
}