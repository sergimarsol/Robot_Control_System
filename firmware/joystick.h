#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

extern volatile uint8_t state_joystick;
extern volatile uint8_t robot_stopped;  // Declare robot_stopped as extern

// Function prototypes
void init_joystick(void);
void init_stop_button(void);
void control_movement_joystick(uint8_t state_joystick, uint8_t speed);

#endif // JOYSTICK_H
