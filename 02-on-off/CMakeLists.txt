cmake_minimum_required(VERSION 3.15)

include(pico_sdk_import.cmake)

project(02-on-off C CXX ASM)

pico_sdk_init()

add_executable(02-on-off
    main.c
    ~/Repositories/pico/mcu/02-on-off/led/led.c  # Вот сюда добавляем путь к led.c
)

pico_enable_stdio_usb(02-on-off 1)
pico_enable_stdio_uart(02-on-off 0)  # Отключаем UART, оставляем только USB

target_link_libraries(02-on-off
    pico_stdlib
    hardware_gpio
)

pico_add_extra_outputs(02-on-off)
