// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include "lcd.h"
#include "i2c.h"
#include "timer.h"
#include <stdio.h>
#include <msp430.h> 

char missatge[17]; // Buffer for LCD messages

void init_LCD(void){
    uint8_t LCD_init[8]; // Array to store LCD initialization data

    /* RESET */
    P2OUT &= ~BIT5;     // Activate Reset
    delay_ms(10);       // Wait 10ms before deactivating Reset
    P2OUT |= BIT5;      // Deactivate Reset
    delay_ms(50);       // Wait 50ms before proceeding

    // Initialization data extracted from the datasheet
    LCD_init[0] = 0x00;
    LCD_init[1] = 0x39;
    LCD_init[2] = 0x14;
    LCD_init[3] = 0x74;
    LCD_init[4] = 0x54;
    LCD_init[5] = 0x6F;
    LCD_init[6] = 0x0C;
    LCD_init[7] = 0x01;

    I2C_send(0x3E, LCD_init, 8);  // Send initialization data to the LCD
    delay_ms(10);                 // Wait 10ms before proceeding
}

void text_LCD(char *data){
    uint8_t length;  // Variable to store the length of the text
    length = sprintf(missatge, data);    // Format the text and store its length
    I2C_send(0x3E, missatge, length);   // Send the text to the LCD via I2C
    delay_ms(10);                       // Wait 10ms before proceeding
}

void new_line_LCD(void){
    missatge[0] = 0x00;
    missatge[1] = 0xC0; // Command to move the cursor to the beginning of the second line
    I2C_send(0x3E, missatge, 2); // Send the command to the LCD
}

void clear_LCD(void){
    char borrar[2];     // Buffer for the clear screen command
    borrar[0] = 0x00;
    borrar[1] = 0x01;   // Command to clear the screen
    I2C_send(0X3E, borrar, 2); // Send the command to the LCD
    delay_ms(10);              // Wait for a short moment
}

void print_LCD(const char *text, uint16_t var) {
    char LCD_send[17]; // Buffer for the formatted message
    uint16_t length;   // Variable to store the length of the formatted text
    length = sprintf(LCD_send, text, var);  // Format the text with a variable
    I2C_send(0x3E, LCD_send, length);      // Send the formatted text to the LCD
}

void print_LCD_two_values(const char *text, uint16_t var1, uint16_t var2) {
    char LCD_send[17]; // Buffer for the formatted message
    uint16_t length;   // Variable to store the length of the formatted text
    length = sprintf(LCD_send, text, var1, var2); // Format the text with two variables
    I2C_send(0x3E, LCD_send, length);            // Send the formatted text to the LCD
}

void write_LCD_one_line(const char *text) {
    clear_LCD(); // Clear the LCD before writing
    text_LCD(text); // Write the text on the first line
    delay_ms(50); // Wait for a short moment
}

void write_LCD_two_lines(const char *line1, const char *line2) {
    clear_LCD(); // Clear the LCD before writing
    text_LCD(line1); // Write the text on the first line
    new_line_LCD(); // Move to the second line
    text_LCD(line2); // Write the text on the second line
    delay_ms(50); // Wait for a short moment
}