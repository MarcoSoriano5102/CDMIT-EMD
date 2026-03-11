#include "PLANTOWER_PMSA003.hpp"

PMSA003::PMSA003(i2c_inst_t *i2c_port)
{
    i2c = i2c_port;
}

void PMSA003::init(uint sda_pin, uint scl_pin)
{
    i2c_init(i2c, 100000);

    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);

    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}

int16_t PMSA003::read_pm25()
{
    uint8_t buffer[14];

    int result = i2c_read_blocking(i2c, PMSA003_ADDR, buffer, sizeof(buffer), false);

    if(result != 14)
        return -1;

    int16_t pm25 = (buffer[12] << 8) | buffer[13];

    return pm25;
}
