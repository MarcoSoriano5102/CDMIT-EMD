cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(CDMIT-EMD)

pico_sdk_init()

add_subdirectory(include)

add_executable(pico_gps
    src/main.cpp
)

target_include_directories(pico_gps PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(pico_gps
    pico_stdlib
    hardware_uart
    AdafruitGPS
    hardware_i2c
    PLANTOWER_PMSA003
)

pico_enable_stdio_usb(pico_gps 1)
pico_enable_stdio_uart(pico_gps 0)

pico_add_extra_outputs(pico_gps)
