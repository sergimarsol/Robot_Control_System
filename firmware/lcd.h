#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// Function prototypes
void init_LCD(void);
void text_LCD(char *data);
void new_line_LCD(void);
void clear_LCD(void);
void print_LCD(const char *text, uint16_t var);
void print_LCD_two_values(const char *text, uint16_t var1, uint16_t var2);
void write_LCD_one_line(const char *text);
void write_LCD_two_lines(const char *line1, const char *line2);

#endif // LCD_H
