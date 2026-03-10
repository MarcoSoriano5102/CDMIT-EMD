#ifndef PLANTOWER_PMSA003_HPP
#define PLANTOWER_PMSA003_HPP

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define PMSA003_ADDR 0x42

class PMSA003
{
public:
    PMSA003(i2c_inst_t *i2c_port);

    void init(uint sda_pin, uint scl_pin);
    int16_t read_pm25();

private:
    i2c_inst_t *i2c;
};

#endif
