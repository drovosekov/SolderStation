/*
 * LCD module with russian codepage translator
 * write 19.07.2015
 * 2San
 * aa@drovosekov.net
 *
 */

#include "main.h"

void set_reset_bit_e();

enum
{
	LCD_2X16_RESET = 0x30, // ������� ������ ���
	//LCD_2X16_4_BIT_BUS = 0x20, // 4-� ��������� ���� ������
	//LCD_2X16_8_BIT_BUS = 0x30, // 8-� ��������� ���� ������
	//LCD_2X16_LINE_ONE = 0x20, // ���� ������
	//LCD_2X16_LINES_TWO = 0x28, // ��� ������
	//LCD_2X16_FONT_5X8 = 0x20, // ����� 5x8 �����
	//LCD_2X16_FONT_5X10 = 0x24, // ����� 5x10 �����
	LCD_2X16_DISPLAY_CLEAR = 0x01, // ������� �������
	//LCD_2X16_DISPLAY_HOME = 0x02, // ��������� ������� ������� ������� � �������
	LCD_2X16_DISPLAY_ON = 0x0C, // �������� �������. ����. � CURSOR_ON � CURSOR_BLINK
	//LCD_2X16_DISPLAY_RIGHT = 0x1C, // �������� ������� ������
	//LCD_2X16_DISPLAY_LEFT = 0x18, // �������� ������� �����
	//LCD_2X16_DISPLAY_SHIFT = 0x05, // ����. � SHIFTDIR_RIGHT � SHIFTDIR_LEFT
	//LCD_2X16_CURSOR_ON = 0x0A, // �������� ������. ����. � DISPLAY_ON � CURSOR_BLINK
	//LCD_2X16_CURSOR_BLINK = 0x09, // ����. � DISPLAY_ON � CURSOR_ON
	//LCD_2X16_CURSOR_RIGHT = 0x14, // �������� ������ ������
	//LCD_2X16_CURSOR_LEFT = 0x10, // �������� ������ �����
	LCD_2X16_SHIFT_RIGHT = 0x06, // ����. � DISPLAY_SHIFT
	//LCD_2X16_SHIFT_LEFT = 0x04, // ����. � DISPLAY_SHIFT
	//LCD_2X16_CGRAM_ADDR = 0x40, // ��������� ������ CGRAM
	LCD_2X16_DDRAM_ADDR = 0x80, // ��������� ������ DDRAM
	//LCD_2X16_BUSY_FLAG = 0x80, // ���� ���������
	LCD_2X16_NEXT_LINE = 0x40, // ����� ����� ������
	LCD_2X16_COMMAND = 0x01,
	LCD_2X16_DATA = 0x00,
} lcd_command;

const u8 lcd_decode[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

const u8 russian_codepage[]={
			0x41, //�
		0x80, //�
			0x42, //�
		0x81, //�
		0x82, //�
			0x45, //�
		0x83, //�
		0x92, //�
		0x84, //�
		0x85, //�
			0x4B, //�
		0x86, //�
			0x4D, //�
			0x48, //�
			0x4F, //�
		0x87, //�
			0x50, //�
			0x43, //�
			0x54, //�
		0x93, //�
		0x88, //�
			0x58, //�
		0x89, //�
		0x8A, //�
		0x8B, //�
		0x8C, //�
		0x8D, //�
		0x8E, //�
		0x94, //�
		0x8F, //�
		0x90, //�
		0x91, //�

			0x61, //�
		0xA0, //�
		0xB5, //�
		0xA1, //�
		0xA2, //�
			0x65, //�
		0xA3, //�
		0xB2, //�
		0xA4, //�
		0xA5, //�
		0xB6, //�
		0xA6, //�
		0xB7, //�
		0xB8, //�
			0x6F, //�
		0xA7, //�
			0x70, //�
			0x63, //�
		0xB9, //�
		0xB3, //�
		0xA8, //�
			0x78, //�
		0xA9, //�
		0xAA, //�
		0xAB, //�
		0xAC, //�
		0xAD, //�
		0xAE, //�
		0xB4, //�
		0xAF, //�
		0xB0, //�
		0xB1  //�
};

void lcd_init(void) {
	delay_ms(15);
	lcd_send_command(LCD_2X16_RESET);
	delay_ms(5);
	lcd_send_command(LCD_2X16_RESET);
	delay_ms(1);
	lcd_send_command(LCD_2X16_RESET);

	lcd_set_4bit_mode();
	lcd_clear();

	lcd_send_command(LCD_2X16_SHIFT_RIGHT);

	lcd_set_state(LCD_ENABLE, CURSOR_DISABLE);

	lcd_init_user_chars();
}

void lcd_init_user_chars(void){
	/*
	u8 uch[]={
				{
					0b00000, // ������
					0b00100, // � ����
					0b01010, // ��������
					0b10001, // ���
					0b10001, // ���������
					0b10001, // ���������
					0b01110,
					0b00000
				},
				{
					0b11000, // ������
					0b11000, // ������ C
					0b00000,
					0b00110,
					0b01001,
					0b01000,
					0b01001,
					0b00110
				},
				{
					0b00100, // ������
					0b10101, // �������
					0b01110, // ������
					0b00100,
					0b00000,
					0b01110,
					0b11111,
					0b11111
				},
				{
					0b11111, // ������
					0b10101, // ������
					0b00000,
					0b00100,
					0b01110,
					0b11111,
					0b11111,
					0b01110
				},
				{
					0b11111, // ������
					0b10101, // ������
					0b00000, // ��� �������
					0b00100,
					0b01110,
					0b10001,
					0b10001,
					0b01110
				},
				{
					0b00000, // ������
					0b01110, // ������ ���������
					0b10001, // �����
					0b10001, // ���
					0b10001, // ���������
					0b10001, // ������ ��
					0b10001, // ������ 0%
					0b11111
				},
				{
					0b00000, // ������
					0b01110, // ���������
					0b10001, // �����
					0b10001, // ���
					0b10001, // ���������
					0b11111, // ������ ��
					0b11111, // ������ 50%
					0b11111
				},
				{
					0b00000, // ������
					0b01110, // ���������
					0b11111, // �����
					0b11111, // ���
					0b11111, // ���������
					0b11111, // ������ ��
					0b11111, // ������ 100%
					0b11111
				}
			};

	for(i=0; i < ARRAY_LENGHT(uch); i++){
		lcd_set_user_char(0, uch[i]);
	}*/

	u8 user_char[8]; //���� ����� ���������� ���������������� ������

	user_char[0]=0b00000; // ������
	user_char[1]=0b00100; // � ����
	user_char[2]=0b01010; // ��������
	user_char[3]=0b10001; // ���
	user_char[4]=0b10001; // ���������
	user_char[5]=0b10001; // ���������
	user_char[6]=0b01110;
	user_char[7]=0b00000;
	lcd_set_user_char(0, user_char);

	user_char[0]=0b11000; // ������
	user_char[1]=0b11000; // ������ C
	user_char[2]=0b00000; //
	user_char[3]=0b00110; //
	user_char[4]=0b01001; //
	user_char[5]=0b01000; //
	user_char[6]=0b01001;
	user_char[7]=0b00110;
	lcd_set_user_char(1, user_char);

	user_char[0]=0b00100; // ������
	user_char[1]=0b10101; // �������
	user_char[2]=0b01110; // ������
	user_char[3]=0b00100; //
	user_char[4]=0b00000; //
	user_char[5]=0b01110; //
	user_char[6]=0b11111;
	user_char[7]=0b11111;
	lcd_set_user_char(2, user_char);

	user_char[0]=0b11111; // ������
	user_char[1]=0b10101; // ������
	user_char[2]=0b00000; //
	user_char[3]=0b00100; //
	user_char[4]=0b01110; //
	user_char[5]=0b11111; //
	user_char[6]=0b11111;
	user_char[7]=0b01110;
	lcd_set_user_char(3, user_char);

	user_char[0]=0b11111; // ������
	user_char[1]=0b10101; // ������
	user_char[2]=0b00000; // ��� �������
	user_char[3]=0b00100; //
	user_char[4]=0b01110; //
	user_char[5]=0b10001; //
	user_char[6]=0b10001;
	user_char[7]=0b01110;
	lcd_set_user_char(4, user_char);


	user_char[0]=0b00000; // ������
	user_char[1]=0b01110; // ������ ���������
	user_char[2]=0b10001; // �����
	user_char[3]=0b10001; // ���
	user_char[4]=0b10001; // ���������
	user_char[5]=0b10001; // ������ ��
	user_char[6]=0b10001; // ������ 0%
	user_char[7]=0b11111;
	lcd_set_user_char(5, user_char);

	user_char[0]=0b00000; // ������
	user_char[1]=0b01110; // ���������
	user_char[2]=0b10001; // �����
	user_char[3]=0b10001; // ���
	user_char[4]=0b10001; // ���������
	user_char[5]=0b11111; // ������ ��
	user_char[6]=0b11111; // ������ 50%
	user_char[7]=0b11111;
	lcd_set_user_char(6, user_char);

	user_char[0]=0b00000; // ������
	user_char[1]=0b01110; // ���������
	user_char[2]=0b11111; // �����
	user_char[3]=0b11111; // ���
	user_char[4]=0b11111; // ���������
	user_char[5]=0b11111; // ������ ��
	user_char[6]=0b11111; // ������ 100%
	user_char[7]=0b11111;
	lcd_set_user_char(7, user_char);

}

void lcd_set_user_char(uint8_t char_num, uint8_t * char_data) {
	u8 i;
	lcd_send_command(((1<<6) | (char_num * 8) ));
	for (i = 0; i < 8 ; i++) {
		lcd_send_data(char_data[i]);
	}
	lcd_send_command((1<<7));
}


void lcd_write_dec_xxxx(uint16_t dig){
	u16 d=dig % 1000;
	lcd_send_data(lcd_decode[(dig / 1000) & 0x0F]);
	lcd_send_data(lcd_decode[(d / 100) & 0x0F]);
	lcd_send_data(lcd_decode[(d % 100) / 10 & 0x0F]);
	lcd_send_data(lcd_decode[(d % 100) % 10 & 0x0F]);
}

void lcd_write_dec_xxx(uint16_t dig){
	lcd_send_data(lcd_decode[(dig / 100) & 0x0F]);
	lcd_send_data(lcd_decode[((dig % 100) / 10) & 0x0F]);
	lcd_send_data(lcd_decode[((dig % 100) % 10) & 0x0F]);
}

void lcd_write_dec_xx(uint8_t dig){
	lcd_send_data(lcd_decode[((dig % 100) / 10) & 0x0F]);
	lcd_send_data(lcd_decode[((dig % 100) % 10) & 0x0F]);
}

void lcd_write_dec_x(uint8_t dig) {
	lcd_send_data(lcd_decode[dig]);
}

void lcd_write_float(float dig){//dig - ������� ����� � ��������� �� ������� �����
	u8 dig_int = dig; //����� ����� ����� �����
	lcd_write_dec_xx(dig_int);

	lcd_out(FLOAT_POINT);

	u8 dig_float = (dig - dig_int) * 10; //����� ����� ������� �����
	lcd_write_dec_x(dig_float);
}

/*�������� �������
 * y = 0 - ������ ������
 * y = 1 - ������ ������
 * x = 0...15*/
void lcd_set_xy(uint8_t x, uint8_t y)  {
	uint8_t position = LCD_2X16_DDRAM_ADDR;

	if (y > 0) {position |= LCD_2X16_NEXT_LINE;}

	lcd_send_command(position | x);
}

void lcd_out(char *STRING)
{
	unsigned char c; //������ �� ������
	while ((c=*STRING++)){
		if(c>=192){
			lcd_send_data(russian_codepage[c-192]);
		}else{
			lcd_send_data(c);
		}
	}
}

void lcd_clear(void) {
	lcd_send_command(LCD_2X16_DISPLAY_CLEAR);
}

void lcd_set_state(lcd_state state, cursor_state cur_state)  {
	if (state == LCD_DISABLE){
		lcd_send_command(0x08);
	}else if (cur_state == CURSOR_DISABLE){
		lcd_send_command(LCD_2X16_DISPLAY_ON);
	}else{
		lcd_send_command(0x0F); //blink cursor
	}
}

void lcd_set_4bit_mode(void) {
	LCD_PORT->BSRR = LCD_ALL_PINS_OFF;
	PIN_ON(LCD_2X16_DB5);
	set_reset_bit_e();

	LCD_PORT->BSRR = LCD_ALL_PINS_OFF;
	PIN_ON(LCD_2X16_DB5);
	set_reset_bit_e();

	LCD_PORT->BSRR = LCD_ALL_PINS_OFF;
	PIN_ON(LCD_2X16_DB7);
	set_reset_bit_e();
}

void lcd_send_command(uint8_t byte){
	lcd_send(byte, COMMAND);
}

void lcd_send_data(uint8_t byte){
	lcd_send(byte, DATA);
}

void lcd_send(uint8_t byte, dat_or_comm dc)  {
	LCD_PORT->BSRR = LCD_ALL_PINS_OFF;

	if (dc) {PIN_ON(LCD_2X16_RS);}

	//���������� ���� ��� �������� ������� �������� �����
	if (byte & 0x10) {PIN_ON(LCD_2X16_DB4);}
	if (byte & 0x20) {PIN_ON(LCD_2X16_DB5);}
	if (byte & 0x40) {PIN_ON(LCD_2X16_DB6);}
	if (byte & 0x80) {PIN_ON(LCD_2X16_DB7);}

	set_reset_bit_e();

	LCD_PORT->BSRR = LCD_ALL_DATA_PINS_OFF;

	//���������� ���� ��� �������� ������ �������� �����
	if (byte & 0x01) {PIN_ON(LCD_2X16_DB4);}
	if (byte & 0x02) {PIN_ON(LCD_2X16_DB5);}
	if (byte & 0x04) {PIN_ON(LCD_2X16_DB6);}
	if (byte & 0x08) {PIN_ON(LCD_2X16_DB7);}

	set_reset_bit_e();
}


//������� ����� ��� ������ ��������
void set_reset_bit_e(){
	PIN_ON(LCD_2X16_E);
	delay_ms(5);
	PIN_OFF(LCD_2X16_E);
}
