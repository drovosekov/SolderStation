#include "main.h"

SLD_INFO sld;
SLD_INFO fen;

void printSolderInfoLCD(u16 *solderT){
	static u16 oldSolderT = 0;
	static u8 dust_clock = 0;

	hd44780_puts("Sld: t");
	if(oldSolderT < *solderT){
		hd44780_write_data(SYMB_UP_ARROW);
	}else{
		hd44780_write_data(SYMB_DN_ARROW);
	}
	lcd_write_dec_auto(*solderT);
	hd44780_write_data(SYMB_DEGREE);
	hd44780_puts("    ");

	if(sld.state == isPreOn || sld.state == isSleepMode){
		hd44780_goto_xy(0, 15);
		dust_clock = (dust_clock==2) ? 3 : 2;
		hd44780_write_data(dust_clock);
	}

	oldSolderT = *solderT;
}

void printFenInfoLCD(u16 *airT){
	static u16 oldAirT = 0;
	static u8 dust_clock = 0;

	hd44780_puts("Fen: ");
	lcd_write_dec_auto(fen.air_flow);
	hd44780_puts("%   ");

	hd44780_goto_xy(1, 10);
	hd44780_puts("t");
	if(oldAirT < *airT){
		hd44780_write_data(SYMB_UP_ARROW);
	}else{
		hd44780_write_data(SYMB_DN_ARROW);
	}
	lcd_write_dec_auto(*airT);
	hd44780_write_data(SYMB_DEGREE);
	hd44780_puts("   ");

	if(fen.state == isPreOn || fen.state == isSleepMode){
		hd44780_goto_xy(1, 15);
		dust_clock = (dust_clock==2) ? 3 : 2;
		hd44780_write_data(dust_clock);
	}

	oldAirT = *airT;
}

void init_user_chars(void){

	u8 user_char[8]; //—юда будем записывать пользовательский символ

	user_char[0]=0b11000; // симовл
	user_char[1]=0b11000; // градус C
	user_char[2]=0b00000; //
	user_char[3]=0b00110; //
	user_char[4]=0b01001; //
	user_char[5]=0b01000; //
	user_char[6]=0b01001;
	user_char[7]=0b00110;

	hd44780_set_user_char(0, user_char);

	user_char[0]=0b00000;
	user_char[1]=0b00100; // стрелка вверх
	user_char[2]=0b01110; //
	user_char[3]=0b10101; //
	user_char[4]=0b00100; //
	user_char[5]=0b00100; //
	user_char[6]=0b00100; //
	user_char[7]=0b00000;

	hd44780_set_user_char(1, user_char);

	user_char[0]=0b11111;
	user_char[1]=0b10001; // песочные часы
	user_char[2]=0b01010; //
	user_char[3]=0b00100; //
	user_char[4]=0b01110; //
	user_char[5]=0b11111; //
	user_char[6]=0b11111; //
	user_char[7]=0b00000;

	hd44780_set_user_char(2, user_char);

	user_char[0]=0b11111;
	user_char[1]=0b11111; // песочные часы
	user_char[2]=0b01110; //
	user_char[3]=0b00100; //
	user_char[4]=0b01010; //
	user_char[5]=0b10001; //
	user_char[6]=0b11111; //
	user_char[7]=0b00000;

	hd44780_set_user_char(3, user_char);

}

void draw_logo(void){
	hd44780_goto_xy(0, 1);
	hd44780_puts("Solder station");
	hd44780_goto_xy(1, 1);
	hd44780_puts("Drovosekov.net");
	delay_ms(AFTER_LOGO_DELAY_ms);
}
