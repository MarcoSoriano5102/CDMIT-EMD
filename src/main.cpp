#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "AdafruitGPS.hpp"
#include "PLANTOWER_PMSA003.hpp"

#define UART_ID uart0
#define BAUD_RATE 9600

#define UART_TX_PIN 12
#define UART_RX_PIN 13

#define GPSECHO true  // Set to true to echo raw GPS data to Serial

AdafruitGPS GPS;

PMSA003 pms(i2c0);

uint32_t timer = to_ms_since_boot(get_absolute_time());

void setup(void) {
    stdio_init_all();

    sleep_ms(2000);

    // Initialize GPS
    GPS.begin(UART_ID, BAUD_RATE, UART_TX_PIN, UART_RX_PIN);

    // Enable RMC and GGA sentences
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // Set update rate to 1Hz
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

    // Request antenna status
    GPS.sendCommand(PGCMD_ANTENNA);

    sleep_ms(2000);

    printf("Pico GPS Reader Started\n");
    
    // Initialize PMSA003
	pms.init(4, 5);
	
	sleep_ms(2000);

    printf("PMSA003 Reader Started\n");
}

int main(void) {
    setup();
    while(1) {
        // Read characters from GPS
        char c = GPS.read();
        int16_t pm25 = pms.read_pm25();
        
        if (GPSECHO && c) printf("%c", c);
        // If a sentence is received, parse it
        if (GPS.newNMEAreceived()) {
            if (!GPS.parse(GPS.lastNMEA())) continue; // Failed to parse
        }
        // Print data every 2 seconds
        if (to_ms_since_boot(get_absolute_time()) - timer > 2000) {
            timer = to_ms_since_boot(get_absolute_time());
            printf("Fix: %d\n", (int)GPS.fix);
            printf("Quality: %d\n", (int)GPS.fixquality);
            if (GPS.fix) {
                printf("Location: %.4f%c, %.4f%c\n",
                    GPS.latitude, GPS.lat, GPS.longitude, GPS.lon);
                printf("Speed (knots): %f\n", GPS.speed);
                printf("Angle: %f\n", GPS.angle);
                printf("Altitude: %f\n", GPS.altitude);
                printf("Satellites: %d\n", (int)GPS.satellites);
                printf("Date/Time: %d/%d/20%d %d:%d:%d\n",
                    GPS.day, GPS.month, GPS.year,
                    GPS.hour, GPS.minute, GPS.seconds);
            }
            printf("PM2.5: %d ug/m3\n", pm25);
        }
    }

    return 0;
}
