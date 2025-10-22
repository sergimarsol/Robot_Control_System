#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>

// Function prototypes
void init_motors(void);
void motors(uint8_t motor1_dir, uint8_t motor1_speed, uint8_t motor2_dir, uint8_t motor2_speed);
void forward(uint8_t speed);
void backward(uint8_t speed);
void turn_left(uint8_t speed);
void turn_right(uint8_t speed);
void stop(void);

#endif // MOTORS_H
