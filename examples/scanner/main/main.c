#include "main.h"

#include "i2c_device.h"
#include <esp_log.h>

static const char *TAG = "example_i2c_scanner";

#define SDA_PORT        GPIO_NUM_21
#define SCL_PORT        GPIO_NUM_22
#define I2C_PORT        I2C_NUM_0
#define I2C_FREQUENCY   400000

esp_err_t i2c_init(void){
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
    ESP_LOGI(TAG, "Scan for devices");
    i2c_init();

    while (1){
        esp_err_t res;
        printf("HEX  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
        printf("00:         ");
        for (uint8_t i = 3; i < 0x78; i++)
        {
            if (i % 16 == 0) printf("\n%.2x:", i);

            i2c_device_info_t i2c_device_info;
            i2c_device_init(&i2c_device_info,I2C_PORT,i);
            res = i2c_device_ping(&i2c_device_info);

            if (res == 0)printf(" %.2x", i);
            else printf(" ..");
        }
        printf("\n\n");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}





