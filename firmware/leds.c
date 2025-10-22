// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include "leds.h"
#include "i2c.h"

#define ADDR 0x10 // I2C address for LEDs

void init_leds(void){
    uint8_t i2c_buffer_leds[3];
    i2c_buffer_leds[0] = 0x0B; // Device identifier: LEDs
    i2c_buffer_leds[1] = WHITE; // Set LED1 to WHITE
    i2c_buffer_leds[2] = WHITE; // Set LED2 to WHITE
    I2C_send(ADDR, i2c_buffer_leds, 3); // Send data via I2C
}

void leds(uint8_t color_led1, uint8_t color_led2){
    uint8_t i2c_buffer_leds[3];
    i2c_buffer_leds[0] = 0x0B; // Device identifier: LEDs
    i2c_buffer_leds[1] = color_led1; // Set LED1 to the specified color
    i2c_buffer_leds[2] = color_led2; // Set LED2 to the specified color
    I2C_send(ADDR, i2c_buffer_leds, 3); // Send data via I2C
}