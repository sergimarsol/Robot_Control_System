#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// Function prototypes
void init_ADC(void);
void start_measure(unsigned int channel_LDR);
void enable_ADC_interrupt(void);
void control_movement_light(uint8_t speed);

#endif // ADC_H
