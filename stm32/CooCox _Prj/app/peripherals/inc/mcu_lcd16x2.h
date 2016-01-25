#include "main.h"

#ifndef _HD44780
#define _HD44780

typedef enum {
	COMMAND = 0,
	DATA = !COMMAND
} dat_or_comm;

typedef enum {
	LCD_DISABLE = 0,
	LCD_ENABLE = !LCD_DISABLE
} lcd_state;

typedef enum {
	CURSOR_DISABLE = 0,
	CURSOR_ENABLE = !CURSOR_DISABLE
} cursor_state;

void lcd_init(void);
void lcd_send(uint8_t byte, dat_or_comm dc);
void lcd_send_command(uint8_t byte);
void lcd_send_data(uint8_t byte);
void lcd_set_4bit_mode(void);
void lcd_set_state(lcd_state state, cursor_state cur_state);
void lcd_clear(void);
void lcd_out(char * txt);
void lcd_set_xy(uint8_t x, uint8_t y);
void lcd_set_user_char(uint8_t char_num, uint8_t * char_data);

void lcd_write_dec_xxxx(uint16_t dig);
void lcd_write_dec_xxx(uint16_t dig);
void lcd_write_dec_xx(uint8_t dig);
void lcd_write_dec_x(uint8_t dig);
void lcd_write_float(float dig);
void lcd_init_user_chars(void);
#endif
