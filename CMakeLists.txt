cmake_minimum_required(VERSION 3.13)
include("/opt/pico-sdk/external/pico_sdk_import.cmake")
project(main C CXX ASM)
pico_sdk_init()
add_executable(main transmit.c) #change to receive.c to test, wipe out build directory, start over
target_link_libraries(main pico_stdlib hardware_gpio hardware_spi)
pico_enable_stdio_usb(main 1)
pico_add_extra_outputs(main)
