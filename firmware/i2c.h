#ifndef I2C_H
#define I2C_H

#include <stdint.h>

// Function prototypes
void i2c_init(void);
void I2C_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades);
void I2C_receive(uint8_t addr, uint8_t *buffer, uint8_t n_dades);

#endif // I2C_H
