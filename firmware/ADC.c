// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include <msp430.h> 
#include "ADC.h"
#include "motors.h"
#include "lcd.h"
#include "timer.h"

volatile uint8_t interr_adc;
volatile uint16_t suma = 0;        // Sum of ADC values for averaging
volatile uint8_t counts = 0;       // Counter for the number of measurements
volatile uint8_t channel_LDR = 10; // Current ADC channel (10 = right, 11 = left)
volatile uint16_t LDR_right = 0;   // Average light intensity for the right channel
volatile uint16_t LDR_left = 0;    // Average light intensity for the left channel
volatile uint8_t update_lcd = 0;   // Flag to indicate LCD update is needed
volatile int16_t diff = 0;  

void init_ADC(void) {
    PM5CTL0 &= ~LOCKLPM5; // Disable high-impedance mode by default

    // Configure P5.2 and P5.3 as analog inputs
    P5SEL0 |= BIT2 | BIT3; // Enable analog functions on P5.2 and P5.3
    P5SEL1 &= ~(BIT2 | BIT3); // Enable analog functions on P5.2 and P5.3

    // ADC Configuration
    ADCCTL0 |= ADCON + ADCSHT_2; // Turn on ADC, S&H=16 cycles
    ADCCTL1 |= ADCSHP;           // Source clock: MODOSC
    ADCCTL2 &= ~ADCRES;          // Clear resolution in ADCCTL2
    ADCCTL2 |= ADCRES_2;         // Set resolution to 12 bits
    ADCIE |= ADCIE0;             // Enable ADC interrupt
}

void start_measure(unsigned int channel_LDR) {
    ADCCTL0 &= ~ADCENC; // Disable the ADC to configure it
    if (channel_LDR == 10) {
        ADCMCTL0 = ADCINCH_10 | ADCSREF_0; // Configure P5.2 as the input channel and use Vcc as the reference
        __no_operation();  // Insert a breakpoint for debugging if needed
    } else if (channel_LDR == 11) {
        ADCMCTL0 = ADCINCH_11 | ADCSREF_0; // Configure P5.3 as the input channel and use Vcc as the reference
        __no_operation();  // Insert a breakpoint for debugging if needed
    }

    ADCCTL0 |= ADCENC | ADCSC; // Enable the ADC and start the conversion
}

void enable_ADC_interrupt(void) {
    ADCIE |= ADCIE0; // Enable ADC interrupt
    start_measure(channel_LDR); // Start a new measurement
}

void control_movement_light(uint8_t speed) {
    // Check if LCD needs to be updated
    if (update_lcd) {
        update_lcd = 0; // Reset the LCD update flag

        // Re-enable the ADC interrupt for the next round of measurements
        ADCIE |= ADCIE0; // Enable ADC interrupt
        start_measure(channel_LDR); // Start a new measurement
    }

    clear_LCD();
    diff = LDR_right - LDR_left; // Calculate the difference between the two channels
    // Determine movement based on light intensity
    if (diff > 250) {
        // Right channel detects more light
        print_LCD_two_values("@L=%d | R=%d", LDR_left, LDR_right); // Display right channel value
        new_line_LCD();
        text_LCD("@RIGHT"); // Display RIGHT on the LCD
        turn_right(speed); // Turn right towards the brighter light
    } else if (diff < -250) {
        // Left channel detects more light
        print_LCD_two_values("@L=%d | R=%d", LDR_left, LDR_right); // Display right channel value
        new_line_LCD();
        text_LCD("@LEFT"); // Display LEFT on the LCD
        turn_left(speed); // Turn left towards the brighter light
    } else {
        // Both channels detect similar light intensity
        print_LCD_two_values("@L=%d | R=%d", LDR_left, LDR_right); // Display right channel value
        new_line_LCD();
        text_LCD("@FORWARD"); // Display FORWARD on the LCD
        forward(speed); // Move straight forward
    }

    delay_ms(500); // Wait for 0.5 seconds to allow the robot to adjust
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void) {
    switch (__even_in_range(ADCIV, ADCIV_ADCIFG)) {
        case ADCIV_ADCIFG: // ADC interrupt flag
            suma += ADCMEM0; // Add the current conversion value to the total sum
            counts++;        // Increment the measurement counter

            if (counts >= 16) { // If 16 measurements have been completed
                if (channel_LDR == 10) {
                    LDR_right = suma / 16; // Calculate the average for the right channel
                    channel_LDR = 11;      // Switch to the left channel
                } else if (channel_LDR == 11) {
                    LDR_left = suma / 16;  // Calculate the average for the left channel
                    channel_LDR = 10;      // Switch to the right channel
                }
                counts = 0; // Reset the counter
                suma = 0;   // Reset the sum
                update_lcd = 1; // Flag to indicate LCD update is needed

                ADCIE &= ~ADCIE0; // Disable ADC interrupt
            }

            start_measure(channel_LDR); // Start a new measurement
            break;

        default:
            break;
    }
}
