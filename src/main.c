#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "soil_moisture.h" 

#ifdef RUN_CALIBRATION_MODE
#include "calibration.h"
#endif

static const char *TAG = "MAIN";

void rhodoshield(void) 
{
    // Initialize the soil moisture component
    if (soil_moisture_init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize soil moisture component.");
        return;
    }

    ESP_LOGI(TAG, "RhodoShield firmware initialized successfully.");

    while (1) {
        int raw = soil_moisture_get_raw();
        float moisture = soil_moisture_get_percentage();

        ESP_LOGI(TAG, "Soil Status -> Raw ADC: %d | Moisture: %.2f%%", raw, moisture);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
#ifdef RUN_CALIBRATION_MODE
    ESP_LOGW("ROUTER", "Calibration mode activated.");
    run_calibration();
#else
    ESP_LOGI("ROUTER", "Standard application mode starting...");
    rhodoshield();
#endif
}