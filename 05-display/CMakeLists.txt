cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico)
set(PICO_PLATFORM rp2040)

include(pico_sdk_import.cmake)

project(02_control)

pico_sdk_init()

add_executable(02_control
	main.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol-task/protocol-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stdio-task/stdio-task.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task/led-task.c
)

add_subdirectory("../libs/protocol" "${PROJECT_BINARY_DIR}/protocol")

add_subdirectory("../libs/ili9341-driver" "${PROJECT_BINARY_DIR}/ili9341-driver")

target_link_libraries(02_control
    pico_stdlib
    ili9341
    hardware_spi
)

pico_set_linker_script(02_control ${CMAKE_CURRENT_SOURCE_DIR}/memmap_rp2040.ld)

pico_enable_stdio_uart(02_control 0)
pico_enable_stdio_usb(02_control 1)

pico_add_extra_outputs(02_control)
