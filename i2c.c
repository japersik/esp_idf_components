#include "i2c.h"
#include "esp_log.h"

static const char * TAG = "i2c_device";


// private api

static bool _is_inited(const i2c_device_info_t * i2c_device_info)
{
    if (i2c_device_info == NULL){
        ESP_LOGE(TAG, "i2c_device_info is NULL");
        return false;
    }
    if (!i2c_device_info->inited){
        ESP_LOGE(TAG, "i2c_device_info (%d) is not inited",i2c_device_info->address);
    }
    return i2c_device_info->inited;
}

static esp_err_t _check_i2c_error(esp_err_t err)
{
    switch (err)
    {
        case ESP_OK:
            break;
        case ESP_ERR_INVALID_ARG:
            ESP_LOGE(TAG, "I2C invalid argument: %d", err);
            break;
        case ESP_FAIL:
            ESP_LOGE(TAG, "I2C fail: %d", err);
            break;
        case ESP_ERR_INVALID_STATE:
            ESP_LOGE(TAG, "I2C invalid state: %d", err);
            break;
        case ESP_ERR_TIMEOUT:
            ESP_LOGE(TAG, "I2C timeout: %d", err);
            break;
        default:
            ESP_LOGE(TAG, "I2C error %d", err);
    }
    return err;
}

// public api

esp_err_t i2c_device_init(i2c_device_info_t * i2c_device_info, i2c_port_t i2c_port, i2c_address_t address){
    if (i2c_device_info == NULL){
        ESP_LOGE(TAG, "i2c_device_info is NULL");
        return ESP_FAIL;
    }
    i2c_device_info->i2c_port = i2c_port;
    i2c_device_info->address = address;
    i2c_device_info->timeout = 100000;
    i2c_device_info->inited = true;

    return ESP_OK;
}


esp_err_t i2c_device_send_byte(const i2c_device_info_t * i2c_device_info, uint8_t data){
    esp_err_t err = ESP_FAIL;
    if (_is_inited(i2c_device_info)){
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, i2c_device_info->address << 1 | I2C_MASTER_WRITE, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, data, I2C_MASTER_ACK);
        i2c_master_stop(cmd);
        err = _check_i2c_error(i2c_master_cmd_begin(i2c_device_info->i2c_port, cmd, i2c_device_info->timeout));
        i2c_cmd_link_delete(cmd);
    }
    return err;
}