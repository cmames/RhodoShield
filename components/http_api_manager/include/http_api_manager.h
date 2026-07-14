#pragma once

#include <esp_http_server.h>
#include <esp_err.h>

/**
 * @brief Initialize and start the HTTP server.
 * @return esp_err_t ESP_OK on success, or appropriate error code.
 */
httpd_handle_t api_webserver_start(void);
