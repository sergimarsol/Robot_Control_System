// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include "sensors.h"
#include "i2c.h"
#include "motors.h"
#include "timer.h"
#include "lcd.h"

#define ADDR 0x10 // I2C address for sensors

volatile uint8_t sensors_out;

void sensors(uint8_t *buffer_out){
    uint8_t i2c_buffer_sensors = 0x1D;
    I2C_send(ADDR, &i2c_buffer_sensors, 1); // Activar
    delay_ms(2);
    I2C_receive(ADDR, buffer_out, 1);
}

void control_movement_lines(uint8_t speed) {
    sensors(&sensors_out); // Read sensor data using the sensors() function

    // Analyze the sensor data and decide the robot's movement
    switch (sensors_out & 0x3F) { // Mask the relevant 6 bits (0x3F -> 00111111)
        case 0x1E: // All 4 sensors detect black (pattern 0x1E → 00011110)
            stop(); // Stop the robot
            write_LCD_one_line("@STOP"); // Display STOP on the LCD
            break;

        case 0x30: // Right sensors detect black (0x30 → 00110000)
        case 0x18: // Right sensors detect black (0x30 → 00011000)
        case 0x10: // Right sensors detect black (0x20 → 00010000)
            turn_right(speed); // Turn right at speed
            write_LCD_one_line("@RIGHT"); // Display RIGHT on the LCD
            break;

        case 0x03: // Left sensors detect black (0x03 → 00000011)
        case 0x06: // Left sensors detect black (0x03 → 00000110)
        case 0x02: // Left sensors detect black (0x02 → 00000010)
            turn_left(speed); // Turn left at speed
            write_LCD_one_line("@LEFT"); // Display LEFT on the LCD
            break;

        case 0x0C: // Center sensors detect black (0x0C → 00001100)
            forward(speed); // Move forward at speed
            write_LCD_one_line("@FORWARD"); // Display FORWARD on the LCD
            break;

        default: // No recognizable signal detected
            stop(); // Stop the robot
            write_LCD_one_line("@STOP"); // Display STOP on the LCD
            break;
    }
}