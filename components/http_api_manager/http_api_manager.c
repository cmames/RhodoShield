#include "http_api_manager.h"
#include "soil_moisture.h"
#include "bme280_manager.h"
#include "actuator_manager.h"
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <esp_http_server.h>

static const char *TAG = "API_MANAGER";

static httpd_handle_t server_handle = NULL;

// HTTP GET handler for URI: /api/status
static esp_err_t status_get_handler(httpd_req_t *req)
{
    actuator_set_led_blue(false);
    char json_response[128];
    
    // Fetch live data from our soil_moisture component API
    int raw_adc = soil_moisture_get_raw();
    float percentage = soil_moisture_get_percentage();
    
    // Fetch weather metrics
    float temp = 0.0f, hum = 0.0f, press = 0.0f;
    bme280_manager_read(&temp, &hum, &press);
    
    // Format data into a standard JSON payload
    snprintf(json_response, sizeof(json_response),
             "{\"soil\":{\"raw\":%d,\"moisture_pct\":%.2f},\"environment\":{\"temperature_c\":%.2f,\"humidity_pct\":%.2f,\"pressure_hpa\":%.2f}}",
             raw_adc, percentage, temp, hum, press);
    // Set HTTP headers and send response
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, json_response, HTTPD_RESP_USE_STRLEN);
    actuator_set_led_blue(true);
    
    return ESP_OK;
}

// URI structure mapping for the HTTP server
static const httpd_uri_t status_uri = {
    .uri       = HTTP_API_URI,
    .method    = HTTP_GET,
    .handler   = status_get_handler,
    .user_ctx  = NULL
};

// Start the lightweight HTTP server
httpd_handle_t api_webserver_start(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    ESP_LOGI(TAG, "Starting HTTP server on port: '%d'", config.server_port);
    if (httpd_start(&server_handle, &config) == ESP_OK) {
        httpd_register_uri_handler(server_handle, &status_uri);
        return server_handle;
    }

    ESP_LOGE(TAG, "Failed to launch HTTP server.");
    return NULL;
}
