#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

// LED color definitions
#define OFF 0x00
#define RED 0x01
#define GREEN 0x02
#define YELLOW 0x03
#define BLUE 0x04
#define PURPLE 0x05
#define PINK 0x06
#define WHITE 0x07

// Function prototypes
void init_leds(void);
void leds(uint8_t color_led1, uint8_t color_led2);

#endif // LEDS_H
