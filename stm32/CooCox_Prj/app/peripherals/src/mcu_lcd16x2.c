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
	LCD_2X16_RESET = 0x30, // Команда сброса ЖКД
	//LCD_2X16_4_BIT_BUS = 0x20, // 4-х разрядная шина данных
	//LCD_2X16_8_BIT_BUS = 0x30, // 8-и разрядная шина данных
	//LCD_2X16_LINE_ONE = 0x20, // Одна строка
	//LCD_2X16_LINES_TWO = 0x28, // Две строки
	//LCD_2X16_FONT_5X8 = 0x20, // Шрифт 5x8 точек
	//LCD_2X16_FONT_5X10 = 0x24, // Шрифт 5x10 точек
	LCD_2X16_DISPLAY_CLEAR = 0x01, // Очистка дисплея
	//LCD_2X16_DISPLAY_HOME = 0x02, // Установка нулевой позиции курсора и дисплея
	LCD_2X16_DISPLAY_ON = 0x0C, // Включить дисплей. Комб. с CURSOR_ON и CURSOR_BLINK
	//LCD_2X16_DISPLAY_RIGHT = 0x1C, // Сдвинуть дисплей вправо
	//LCD_2X16_DISPLAY_LEFT = 0x18, // Сдвинуть дисплей влево
	//LCD_2X16_DISPLAY_SHIFT = 0x05, // Комб. с SHIFTDIR_RIGHT и SHIFTDIR_LEFT
	//LCD_2X16_CURSOR_ON = 0x0A, // Включить курсор. Комб. с DISPLAY_ON и CURSOR_BLINK
	//LCD_2X16_CURSOR_BLINK = 0x09, // Комб. с DISPLAY_ON и CURSOR_ON
	//LCD_2X16_CURSOR_RIGHT = 0x14, // Сдвинуть курсор вправо
	//LCD_2X16_CURSOR_LEFT = 0x10, // Сдвинуть курсор влево
	LCD_2X16_SHIFT_RIGHT = 0x06, // Комб. с DISPLAY_SHIFT
	//LCD_2X16_SHIFT_LEFT = 0x04, // Комб. с DISPLAY_SHIFT
	//LCD_2X16_CGRAM_ADDR = 0x40, // Установка адреса CGRAM
	LCD_2X16_DDRAM_ADDR = 0x80, // Установка адреса DDRAM
	//LCD_2X16_BUSY_FLAG = 0x80, // Флаг занятости
	LCD_2X16_NEXT_LINE = 0x40, // Адрес новой строки
	LCD_2X16_COMMAND = 0x01,
	LCD_2X16_DATA = 0x00,
} lcd_command;

const u8 lcd_decode[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

const u8 russian_codepage[]={
			0x41, //А
		0x80, //Б
			0x42, //В
		0x81, //Г
		0x82, //Д
			0x45, //Е
		0x83, //Ж
		0x92, //З
		0x84, //И
		0x85, //Й
			0x4B, //К
		0x86, //Л
			0x4D, //М
			0x48, //Н
			0x4F, //О
		0x87, //П
			0x50, //Р
			0x43, //С
			0x54, //Т
		0x93, //Ф
		0x88, //У
			0x58, //Х
		0x89, //Ц
		0x8A, //Ч
		0x8B, //Ш
		0x8C, //Щ
		0x8D, //Ъ
		0x8E, //Ы
		0x94, //Ь
		0x8F, //Э
		0x90, //Ю
		0x91, //Я

			0x61, //а
		0xA0, //б
		0xB5, //в
		0xA1, //г
		0xA2, //д
			0x65, //е
		0xA3, //ж
		0xB2, //з
		0xA4, //и
		0xA5, //й
		0xB6, //к
		0xA6, //л
		0xB7, //м
		0xB8, //н
			0x6F, //о
		0xA7, //п
			0x70, //р
			0x63, //с
		0xB9, //т
		0xB3, //ф
		0xA8, //у
			0x78, //х
		0xA9, //ц
		0xAA, //ч
		0xAB, //ш
		0xAC, //щ
		0xAD, //ъ
		0xAE, //ы
		0xB4, //ь
		0xAF, //э
		0xB0, //ю
		0xB1  //я
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
					0b00000, // симовл
					0b00100, // в виде
					0b01010, // капельки
					0b10001, // для
					0b10001, // индикации
					0b10001, // влажности
					0b01110,
					0b00000
				},
				{
					0b11000, // символ
					0b11000, // градус C
					0b00000,
					0b00110,
					0b01001,
					0b01000,
					0b01001,
					0b00110
				},
				{
					0b00100, // симовл
					0b10101, // ручного
					0b01110, // режима
					0b00100,
					0b00000,
					0b01110,
					0b11111,
					0b11111
				},
				{
					0b11111, // символ
					0b10101, // полива
					0b00000,
					0b00100,
					0b01110,
					0b11111,
					0b11111,
					0b01110
				},
				{
					0b11111, // симовл
					0b10101, // полива
					0b00000, // без заливки
					0b00100,
					0b01110,
					0b10001,
					0b10001,
					0b01110
				},
				{
					0b00000, // симовл
					0b01110, // пустой батарейки
					0b10001, // часов
					0b10001, // для
					0b10001, // индикации
					0b10001, // уровня ее
					0b10001, // заряда 0%
					0b11111
				},
				{
					0b00000, // симовл
					0b01110, // батарейки
					0b10001, // часов
					0b10001, // для
					0b10001, // индикации
					0b11111, // уровня ее
					0b11111, // заряда 50%
					0b11111
				},
				{
					0b00000, // симовл
					0b01110, // батарейки
					0b11111, // часов
					0b11111, // для
					0b11111, // индикации
					0b11111, // уровня ее
					0b11111, // заряда 100%
					0b11111
				}
			};

	for(i=0; i < ARRAY_LENGHT(uch); i++){
		lcd_set_user_char(0, uch[i]);
	}*/

	u8 user_char[8]; //Сюда будем записывать пользовательский символ

	user_char[0]=0b00000; // симовл
	user_char[1]=0b00100; // в виде
	user_char[2]=0b01010; // капельки
	user_char[3]=0b10001; // для
	user_char[4]=0b10001; // индикации
	user_char[5]=0b10001; // влажности
	user_char[6]=0b01110;
	user_char[7]=0b00000;
	lcd_set_user_char(0, user_char);

	user_char[0]=0b11000; // симовл
	user_char[1]=0b11000; // градус C
	user_char[2]=0b00000; //
	user_char[3]=0b00110; //
	user_char[4]=0b01001; //
	user_char[5]=0b01000; //
	user_char[6]=0b01001;
	user_char[7]=0b00110;
	lcd_set_user_char(1, user_char);

	user_char[0]=0b00100; // симовл
	user_char[1]=0b10101; // ручного
	user_char[2]=0b01110; // режима
	user_char[3]=0b00100; //
	user_char[4]=0b00000; //
	user_char[5]=0b01110; //
	user_char[6]=0b11111;
	user_char[7]=0b11111;
	lcd_set_user_char(2, user_char);

	user_char[0]=0b11111; // символ
	user_char[1]=0b10101; // полива
	user_char[2]=0b00000; //
	user_char[3]=0b00100; //
	user_char[4]=0b01110; //
	user_char[5]=0b11111; //
	user_char[6]=0b11111;
	user_char[7]=0b01110;
	lcd_set_user_char(3, user_char);

	user_char[0]=0b11111; // симовл
	user_char[1]=0b10101; // полива
	user_char[2]=0b00000; // без заливки
	user_char[3]=0b00100; //
	user_char[4]=0b01110; //
	user_char[5]=0b10001; //
	user_char[6]=0b10001;
	user_char[7]=0b01110;
	lcd_set_user_char(4, user_char);


	user_char[0]=0b00000; // симовл
	user_char[1]=0b01110; // пустой батарейки
	user_char[2]=0b10001; // часов
	user_char[3]=0b10001; // для
	user_char[4]=0b10001; // индикации
	user_char[5]=0b10001; // уровня ее
	user_char[6]=0b10001; // заряда 0%
	user_char[7]=0b11111;
	lcd_set_user_char(5, user_char);

	user_char[0]=0b00000; // симовл
	user_char[1]=0b01110; // батарейки
	user_char[2]=0b10001; // часов
	user_char[3]=0b10001; // для
	user_char[4]=0b10001; // индикации
	user_char[5]=0b11111; // уровня ее
	user_char[6]=0b11111; // заряда 50%
	user_char[7]=0b11111;
	lcd_set_user_char(6, user_char);

	user_char[0]=0b00000; // симовл
	user_char[1]=0b01110; // батарейки
	user_char[2]=0b11111; // часов
	user_char[3]=0b11111; // для
	user_char[4]=0b11111; // индикации
	user_char[5]=0b11111; // уровня ее
	user_char[6]=0b11111; // заряда 100%
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

void lcd_write_float(float dig){//dig - дробное число с точностью до десятых долей
	u8 dig_int = dig; //здесь будет целая часть
	lcd_write_dec_xx(dig_int);

	lcd_out(FLOAT_POINT);

	u8 dig_float = (dig - dig_int) * 10; //здесь будет дробная часть
	lcd_write_dec_x(dig_float);
}

/*Положеня курсора
 * y = 0 - первая строка
 * y = 1 - вторая строка
 * x = 0...15*/
void lcd_set_xy(uint8_t x, uint8_t y)  {
	uint8_t position = LCD_2X16_DDRAM_ADDR;

	if (y > 0) {position |= LCD_2X16_NEXT_LINE;}

	lcd_send_command(position | x);
}

void lcd_out(char *STRING)
{
	unsigned char c; //символ из строки
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

	//выставляем пины для передачи верхней половины байта
	if (byte & 0x10) {PIN_ON(LCD_2X16_DB4);}
	if (byte & 0x20) {PIN_ON(LCD_2X16_DB5);}
	if (byte & 0x40) {PIN_ON(LCD_2X16_DB6);}
	if (byte & 0x80) {PIN_ON(LCD_2X16_DB7);}

	set_reset_bit_e();

	LCD_PORT->BSRR = LCD_ALL_DATA_PINS_OFF;

	//выставляем пины для передачи нижней половины байта
	if (byte & 0x01) {PIN_ON(LCD_2X16_DB4);}
	if (byte & 0x02) {PIN_ON(LCD_2X16_DB5);}
	if (byte & 0x04) {PIN_ON(LCD_2X16_DB6);}
	if (byte & 0x08) {PIN_ON(LCD_2X16_DB7);}

	set_reset_bit_e();
}


//дергаем ногой для записи значений
void set_reset_bit_e(){
	PIN_ON(LCD_2X16_E);
	delay_ms(5);
	PIN_OFF(LCD_2X16_E);
}
