#include "i2c_device.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static const char * TAG = "i2c_device";

// Private api

// Mutexes to protect against race conditions on access to i2c ports
static SemaphoreHandle_t mutex[I2C_NUM_MAX];

static SemaphoreHandle_t takeMutex(i2c_port_t port){
    if (mutex[port] == NULL) {
        ESP_LOGI(TAG, "Mutex for port %d created", port);
        mutex[port] = xSemaphoreCreateMutex();
    }
    return mutex[port];
}

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

// Public api

esp_err_t i2c_device_init(i2c_device_info_t * i2c_device_info, i2c_port_t i2c_port, i2c_address_t address){
    if (i2c_device_info == NULL){
        ESP_LOGE(TAG, "i2c_device_info is NULL");
        return ESP_FAIL;
    }
    i2c_device_info->i2c_port = i2c_port;
    i2c_device_info->address = address;
    i2c_device_info->timeout = 50000;
    i2c_device_info->inited = true;

    return ESP_OK;
}


esp_err_t i2c_device_send_byte(const i2c_device_info_t * i2c_device_info, uint8_t data){
    esp_err_t err = ESP_FAIL;
    if (xSemaphoreTake(takeMutex(i2c_device_info->i2c_port), portMAX_DELAY)) {
        if (_is_inited(i2c_device_info)){
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, i2c_device_info->address << 1 | I2C_MASTER_WRITE, true);
            i2c_master_write_byte(cmd, data, true);
            i2c_master_stop(cmd);
            err = _check_i2c_error(i2c_master_cmd_begin(i2c_device_info->i2c_port, cmd, i2c_device_info->timeout));
            i2c_cmd_link_delete(cmd);
        }
        xSemaphoreGive(takeMutex(i2c_device_info->i2c_port));
    }
    return err;
}

esp_err_t i2c_device_receive_byte(const i2c_device_info_t * i2c_device_info, uint8_t * data){
    esp_err_t err = ESP_FAIL;
    if (xSemaphoreTake(takeMutex(i2c_device_info->i2c_port), portMAX_DELAY)) {
        if (_is_inited(i2c_device_info)) {
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, i2c_device_info->address << 1 | I2C_MASTER_READ, true);
            i2c_master_read_byte(cmd, data, I2C_MASTER_NACK);
            i2c_master_stop(cmd);
            err = _check_i2c_error(i2c_master_cmd_begin(i2c_device_info->i2c_port, cmd, i2c_device_info->timeout));
            i2c_cmd_link_delete(cmd);
        }
        xSemaphoreGive(takeMutex(i2c_device_info->i2c_port));
    }
    return err;
}

esp_err_t i2c_device_send_bytes(const i2c_device_info_t * i2c_device_info, const uint8_t * data_pkg, size_t len){
    esp_err_t err = ESP_FAIL;
    if (xSemaphoreTake(takeMutex(i2c_device_info->i2c_port), portMAX_DELAY)) {
        if (_is_inited(i2c_device_info)){
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, i2c_device_info->address << 1 | I2C_MASTER_WRITE, true);
            i2c_master_write(cmd, data_pkg,len, true);
            i2c_master_stop(cmd);
            err = _check_i2c_error(i2c_master_cmd_begin(i2c_device_info->i2c_port, cmd, i2c_device_info->timeout));
            i2c_cmd_link_delete(cmd);
        }
        xSemaphoreGive(takeMutex(i2c_device_info->i2c_port));
    }
    return err;
}

esp_err_t i2c_device_receive_bytes(const i2c_device_info_t * i2c_device_info, uint8_t * data_pkg, size_t len){
    esp_err_t err = ESP_FAIL;
    if (xSemaphoreTake(takeMutex(i2c_device_info->i2c_port), portMAX_DELAY)) {
        if (_is_inited(i2c_device_info)){
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, i2c_device_info->address << 1 | I2C_MASTER_READ, true);
            i2c_master_read(cmd, data_pkg,len, I2C_MASTER_NACK);
            i2c_master_stop(cmd);
            err = _check_i2c_error(i2c_master_cmd_begin(i2c_device_info->i2c_port, cmd, i2c_device_info->timeout));
            i2c_cmd_link_delete(cmd);
        }
        xSemaphoreGive(takeMutex(i2c_device_info->i2c_port));
    }
    return err;
}

esp_err_t i2c_device_ping(const i2c_device_info_t * i2c_device_info){
    esp_err_t err = ESP_FAIL;
    if (xSemaphoreTake(takeMutex(i2c_device_info->i2c_port), portMAX_DELAY)) {
        if (_is_inited(i2c_device_info)){
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, i2c_device_info->address << 1  | I2C_MASTER_WRITE, true);
            i2c_master_stop(cmd);
            err = i2c_master_cmd_begin(i2c_device_info->i2c_port, cmd, i2c_device_info->timeout);
            i2c_cmd_link_delete(cmd);
        }
        xSemaphoreGive(takeMutex(i2c_device_info->i2c_port));
    }
    return err;
}