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
}

int16_t PMSA003::read_pm25()
{
    uint8_t buffer[14];

    int result = i2c_read_blocking(i2c, PMSA003_ADDR, buffer, 14, false);

    if(result != 14)
        return -1;

    int16_t pm25 = (buffer[12] << 8) | buffer[13];

    return pm25;
}
