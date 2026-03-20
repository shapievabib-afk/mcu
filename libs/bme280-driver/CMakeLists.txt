cmake_minimum_required(VERSION 3.15)

project(bme280_driver)

add_library(bme280_driver STATIC
    bme280-driver.c
)

target_include_directories(bme280_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
