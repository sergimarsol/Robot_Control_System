// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include <msp430.h> 
#include <stdint.h>
#include <stdio.h>

#include "ADC.h"
#include "timer.h"
#include "i2c.h"
#include "leds.h"
#include "lcd.h"
#include "motors.h"
#include "sensors.h"
#include "joystick.h"

/**
 * main.c
 * This file contains the main function for the robot control system.
 * It initializes the necessary components, displays the main menu on the LCD,
 * and allows the user to select a mode of operation using the joystick.
 */
main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // initializations
    init_clocks();
    init_timers();
    i2c_init();
    init_leds();
    init_motors();
    init_joystick();
    init_stop_button();
    init_ADC();
    start_measure(10);
    start_measure(11);

    __enable_interrupt();

    init_LCD(); // Initialize the LCD

    write_LCD_two_lines("@Main Menu", "@Sergi Marsol"); // Display the main menu on the LCD
    delay_ms(3000); // Wait for 1 second to display the goodbye message
   
    uint8_t mode = 0; // Current mode (0 = joystick, 1 = follow lines, 2 = follow light)
    uint8_t selected_mode = 0; // Selected mode

    // Mode selection loop
    while (1) {
        if (state_joystick == 2) { // Joystick moved down
            mode = (mode + 1) % 3; // Cycle through modes (0 -> 1 -> 2 -> 0)
            switch (mode) {
                case 0:
                    write_LCD_one_line("@Joystick Control");
                    leds(RED, RED); // Set both LEDs to RED for joystick control mode
                    break;
                case 1:
                    write_LCD_one_line("@Follow Lines");
                    leds(GREEN, GREEN); // Set both LEDs to GREEN for follow lines mode
                    break;
                case 2:
                    write_LCD_one_line("@Follow Light");
                    leds(BLUE, BLUE); // Set both LEDs to BLUE for follow light mode
                    break;
            }
            delay_ms(500); // Debounce delay
            state_joystick = 255; // Reset joystick state to neutral
        } else if (state_joystick == 1) { // Joystick moved up
            mode = (mode + 2) % 3; // Cycle backward through modes (0 -> 2 -> 1 -> 0)
            switch (mode) {
                case 0:
                    write_LCD_one_line("@Joystick Control");
                    leds(RED, RED); // Set both LEDs to RED for joystick control mode
                    break;
                case 1:
                    write_LCD_one_line("@Follow Lines");
                    leds(GREEN, GREEN); // Set both LEDs to GREEN for follow lines mode
                    break;
                case 2:
                    write_LCD_one_line("@Follow Light");
                    leds(BLUE, BLUE); // Set both LEDs to BLUE for follow light mode
                    break;
            }
            delay_ms(500); // Debounce delay
            state_joystick = 255; // Reset joystick state to neutral
        } else if (state_joystick == 0) { // Joystick button pressed
            selected_mode = mode; // Select the current mode
            break; // Exit the mode selection loop
        }
    }

    // Infinite loop for the selected mode
    while (1) {
        switch (selected_mode) {
            case 0: // Joystick control mode
                control_movement_joystick(state_joystick, 50);
                break;
            case 1: // Follow lines mode
                control_movement_lines(50);
                break;
            case 2: // Follow light mode
                control_movement_light(50);
                break;
        }
        delay_ms(100); // Small delay to prevent excessive CPU usage
    }
}
