idf_component_register(
    SRCS "network_manager.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi esp_event mdns esp_http_server nvs_flash soil_moisture bme280_manager
)