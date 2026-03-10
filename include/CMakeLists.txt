add_library(AdafruitGPS STATIC
    AdafruitGPS.cpp
    NMEA_build.cpp
    NMEA_parse.cpp
)

add_library(NMEA_data STATIC
    NMEA_data.cpp
)

add_library(Adafruit_PMTK INTERFACE)

add_library(PLANTOWER_PMSA003 STATIC
	PLANTOWER_PMSA003.cpp
)

# Make headers available to consumers
target_include_directories(AdafruitGPS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(Adafruit_PMTK INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(PLANTOWER_PMSA003 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AdafruitGPS
    pico_stdlib
    hardware_uart
    Adafruit_PMTK
    NMEA_data
    PLANTOWER_PMSA003
)

target_link_libraries(NMEA_data
    pico_stdlib
)

target_link_libraries(PLANTOWER_PMSA003
    pico_stdlib
    hardware_i2c
)
