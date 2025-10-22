// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include "motors.h"
#include "i2c.h"

#define ADDR 0x10 // I2C address for motors

void init_motors(void){
    uint8_t i2c_buffer_motors[5];
    i2c_buffer_motors[0]=0x00; //device: motors
    i2c_buffer_motors[1]=1;
    i2c_buffer_motors[2]=0;
    i2c_buffer_motors[3]=1;
    i2c_buffer_motors[4]=0;
    I2C_send(ADDR, i2c_buffer_motors, 5); // Enviar dades
}

void motors(uint8_t motor1_dir, uint8_t motor1_speed, uint8_t motor2_dir, uint8_t motor2_speed) {
    uint8_t i2c_buffer_motors[5];
    i2c_buffer_motors[0] = 0x00; // Device: motors
    i2c_buffer_motors[1] = motor1_dir;    // Motor 1 direction
    i2c_buffer_motors[2] = motor1_speed;  // Motor 1 speed
    i2c_buffer_motors[3] = motor2_dir;    // Motor 2 direction
    i2c_buffer_motors[4] = motor2_speed;  // Motor 2 speed
    I2C_send(ADDR, i2c_buffer_motors, 5); // Send data to motors
}

void forward(uint8_t speed) {
    motors(1, speed, 1, speed); // Move both motors forward
}

void backward(uint8_t speed) {
    motors(2, speed, 2, speed); // Move both motors backward
}

void turn_left(uint8_t speed) {
    motors(0, speed, 1, speed); // Turn left
}  

void turn_right(uint8_t speed) {
    motors(1, speed, 0, speed); // Turn right
}

void stop(void) {
    motors(0, 0, 0, 0); // Stop both motors
}
