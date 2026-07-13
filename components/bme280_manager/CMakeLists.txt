cmake_minimum_required(VERSION 3.16)

idf_component_register(
    SRCS "bme280_manager.c"
    INCLUDE_DIRS "include"
    REQUIRES driver bme280
)