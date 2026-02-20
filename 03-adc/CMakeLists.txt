cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico)
set(PICO_PLATFORM rp2040)

include(pico_sdk_import.cmake)

project(03_adc)

pico_sdk_init()

add_executable(03_adc
	main.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol-task/protocol-task.c
	${CMAKE_CURRENT_SOURCE_DIR}/adc-task/adc-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stdio-task/stdio-task.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task/led-task.c
)

target_link_libraries(03_adc
    pico_stdlib
    hardware_adc
)

pico_set_linker_script(03_adc ${CMAKE_CURRENT_SOURCE_DIR}/memmap_rp2040.ld)

pico_enable_stdio_uart(03_adc 0)
pico_enable_stdio_usb(03_adc 1)

pico_add_extra_outputs(03_adc)
