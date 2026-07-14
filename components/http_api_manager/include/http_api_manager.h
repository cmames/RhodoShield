// Copyright (c) 2026 C. Mames - Licensed under the GNU GPL v3
#pragma once

#include <esp_err.h>

/**
 * @brief Start the HTTP REST API server.
 * @return esp_err_t ESP_OK on success, or ESP_FAIL.
 */
esp_err_t api_webserver_start(void);