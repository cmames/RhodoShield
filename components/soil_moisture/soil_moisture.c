// Copyright (c) 2026 C. Mames - Licensed under the GNU GPL v3
#include "soil_moisture.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_check.h>
#include <limits.h>

static const char *TAG = "SOIL_MOISTURE";

// Oversampling configuration for software noise reduction
#define OVERSAMPLING_SAMPLES    16

static adc_oneshot_unit_handle_t adc1_handle = NULL;

esp_err_t soil_moisture_init(void) {
    // 1. Initialize ADC Unit
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = MOISTURE_ADC_UNIT,
        .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
    };
    ESP_RETURN_ON_ERROR(adc_oneshot_new_unit(&init_config, &adc1_handle), TAG, "Failed to init ADC unit");

    // 2. Configure ADC Channel (12dB attenuation for full range up to 3.3V)
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    ESP_RETURN_ON_ERROR(adc_oneshot_config_channel(adc1_handle, MOISTURE_ADC_CHANNEL, &config), TAG, "Failed to config ADC channel");

    return ESP_OK;
}

int soil_moisture_get_raw(void) {
    int raw_val = 0;
    if (adc1_handle != NULL) {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, MOISTURE_ADC_CHANNEL, &raw_val));
    }
    return raw_val;
}

float soil_moisture_get_percentage(void) {
    long accumulated_raw = 0;
    int raw_reading = 0;
    int raw_min = INT_MAX;
    int raw_max = 0;
    // Apply oversampling to smooth out analog noise
    for (int i = 0; i < OVERSAMPLING_SAMPLES; i++) {
        raw_reading = soil_moisture_get_raw();
        if (raw_reading > raw_max) raw_max=raw_reading;
        if (raw_reading < raw_min) raw_min=raw_reading;
        accumulated_raw += raw_reading;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    accumulated_raw = accumulated_raw - raw_max - raw_min;
    float mean_raw = (float)accumulated_raw / (OVERSAMPLING_SAMPLES - 2);

    // Constrain the reading within calibrated bounds
    if (mean_raw > MOISTURE_ADC_MAX_DRY) mean_raw = MOISTURE_ADC_MAX_DRY;
    if (mean_raw < MOISTURE_ADC_MIN_WET) mean_raw = MOISTURE_ADC_MIN_WET;

    // Map linearly: MIN_WET -> 100%, MAX_DRY -> 0%
    float percentage = ((float)(MOISTURE_ADC_MAX_DRY - mean_raw) / (MOISTURE_ADC_MAX_DRY - MOISTURE_ADC_MIN_WET)) * 100.0f;

    return percentage;
}


