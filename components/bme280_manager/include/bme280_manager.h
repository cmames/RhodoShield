// Copyright (c) 2026 C. Mames - Licensed under the GNU GPL v3
#pragma once

#include <esp_err.h>

/**
 * @brief Initialize the I2C bus and the BME280 sensor.
 * @return esp_err_t ESP_OK on success, or appropriate error code.
 */
esp_err_t bme280_manager_init(void);

/**
 * @brief Read data from the BME280 sensor.
 * @param temperature Pointer to store the temperature in Celsius.
 * @param humidity    Pointer to store the relative humidity percentage.
 * @param pressure    Pointer to store the pressure in Hectopascals (hPa).
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t bme280_manager_read(float *temperature, float *humidity, float *pressure);