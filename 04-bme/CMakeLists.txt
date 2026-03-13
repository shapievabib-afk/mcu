cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico)
set(PICO_PLATFORM rp2040)

include(pico_sdk_import.cmake)

project(04_bme)

pico_sdk_init()

add_executable(04_bme
	main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stdio-task/stdio-task.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task/led-task.c
)

add_subdirectory("../libs/protocol" "${PROJECT_BINARY_DIR}/protocol")

target_link_libraries(04_bme
    pico_stdlib
    protocol
)

pico_set_linker_script(04_bme ${CMAKE_CURRENT_SOURCE_DIR}/memmap_rp2040.ld)

pico_enable_stdio_uart(04_bme 0)
pico_enable_stdio_usb(04_bme 1)

pico_add_extra_outputs(04_bme)
