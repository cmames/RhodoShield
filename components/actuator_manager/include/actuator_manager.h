#pragma once

#include <esp_err.h>
#include <stdbool.h>

/**
 * @brief Initialize all GPIO pins for the pump and the three status LEDs.
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t actuator_manager_init(void);

/**
 * @brief Control the water pump state.
 * @param enable true to turn pump ON, false to turn OFF.
 */
void actuator_set_pump(bool enable);

/**
 * @brief Control the Blue LED (WiFi/Network status).
 * @param enable true to turn LED ON, false to turn OFF.
 */
void actuator_set_led_blue(bool enable);

/**
 * @brief Control the Yellow LED (Moisture warning status).
 * @param enable true to turn LED ON, false to turn OFF.
 */
void actuator_set_led_yellow(bool enable);

/**
 * @brief Control the Red LED (Critical error / Empty tank status).
 * @param enable true to turn LED ON, false to turn OFF.
 */
void actuator_set_led_red(bool enable);