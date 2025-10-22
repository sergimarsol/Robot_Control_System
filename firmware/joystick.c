// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include "joystick.h"
#include "motors.h"
#include "lcd.h"
#include <msp430.h>

volatile uint8_t state_joystick; // Variable to define joystick state
volatile uint8_t robot_stopped = 0; 

void init_joystick(void){
    P2DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Define pins as inputs
    P2REN |= BIT0 | BIT1 | BIT2 | BIT3;    // Enable pull-up/pull-down resistors
    P2OUT |= BIT0 | BIT1 | BIT2 | BIT3;    // Set pull-up resistors

    // Initialize interrupts
    P2IES |= BIT0 | BIT1 | BIT2 | BIT3;    // Configure interrupts on falling edge
    P2IE |= BIT0 | BIT1 | BIT2 | BIT3;     // Enable interrupts
    P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Clear interrupt flags

    state_joystick = 1; // Set default state to "Forward"
    robot_stopped = 0;  // Initialize robot_stopped to 0 (not stopped)
}

void init_stop_button(void) {
    P1DIR &= ~BIT5; // Set P1.5 as input
    P1REN |= BIT5;  // Enable pull-up/pull-down resistor
    P1OUT |= BIT5;  // Set pull-up resistor

    P1IES |= BIT5;  // Trigger interrupt on falling edge
    P1IE |= BIT5;   // Enable interrupt for P1.5
    P1IFG &= ~BIT5; // Clear interrupt flag for P1.5
}

void control_movement_joystick(uint8_t state_joystick, uint8_t speed) {
    switch (state_joystick) {
        case 0: // Stop
            stop(); // Stop the robot
            write_LCD_one_line("@STOP"); // Display STOP on the LCD
            robot_stopped = 1; // Set robot_stopped to 1 (stopped)
            break;

        case 1: // Forward
            forward(speed); // Move forward
            write_LCD_one_line("@FORWARD"); // Display FORWARD on the LCD
            robot_stopped = 0; // Set robot_stopped to 0 (moving)
            break;

        case 2: // Backward
            backward(speed); // Move backward
            write_LCD_one_line("@BACKWARD"); // Display BACKWARD on the LCD
            robot_stopped = 0; // Set robot_stopped to 0 (moving)
            break;

        case 3: // Turn right
            turn_right(speed); // Turn right
            write_LCD_one_line("@RIGHT"); // Display RIGHT on the LCD
            robot_stopped = 0; // Set robot_stopped to 0 (moving)
            break;

        case 4: // Turn left
            turn_left(speed); // Turn left
            write_LCD_one_line("@LEFT"); // Display LEFT on the LCD
            robot_stopped = 0; // Set robot_stopped to 0 (moving)
            break;

        default:
            stop(); // Stop the robot if no movement is detected
            write_LCD_one_line("@STOP"); // Display STOP on the LCD
            robot_stopped = 1; // Set robot_stopped to 1 (stopped)
            break;
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void joystick(void){
    uint8_t flag = P2IV; // Comprovem com esta la flag
    P2IE &= ~(BIT0 | BIT1 | BIT2 | BIT3); // habilitem les interrupcions

    switch(flag){
        case 0x02: // Joystick endavant
            state_joystick = 1;
            break;
        case 0x04: // Joystick enrere
            state_joystick = 2; // Joystick enrere
            break;
        case 0x06: // Joystick dreta
            state_joystick = 3;
            break;
        case 0x08: // Joystick esquerra
            state_joystick = 4;
            break;
        default:
            state_joystick = 0; // No movement detected
            break;
    }

    // Clear the interrupt flag
    P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3);

    // Re-enable interrupts
    P2IE |= BIT0 | BIT1 | BIT2 | BIT3;
}

#pragma vector=PORT1_VECTOR
__interrupt void stop_motor(void) {
    uint8_t flag = P1IV; // Check which pin triggered the interrupt

    if (flag == P1IV_P1IFG5) { // If P1.5 triggered the interrupt
        state_joystick = 0; // Stop the robot
        //stop(); // Call the function to stop the motors
    }

    // Clear the interrupt flag (already cleared by reading P1IV)
}
