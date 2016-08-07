#include "main.h"

SLD_INFO sld;
SLD_INFO fen;
EncBtnStates encBtn;
s8 cursor_cnt_state;

void printSolderInfoLCD(u16 *solderT){
	//static u16 oldSolderT = -1;
	//static u8 dust_clock = 0;
	//static u8 set = 0;

	hd44780_puts("Sld: ");
	print_temp_val(solderT, &sld.temp, (encBtn == SLD_TEMP));
	/*hd44780_write_data(SYMB_TERM);
	if(cursor_cnt_state && encBtn == SLD_TEMP){
		hd44780_write_data((set) ? SYMB_SET_ARROW : 20);
		lcd_write_dec_auto(sld.temp);
	}else if(oldSolderT < *solderT){
		hd44780_write_data(SYMB_UP_ARROW);
		lcd_write_dec_auto(*solderT);
	}else{
		hd44780_write_data(SYMB_DN_ARROW);
		lcd_write_dec_auto(*solderT);
	}

	hd44780_write_data(SYMB_DEGREE);*/
	hd44780_puts("        ");

	//oldSolderT = *solderT;
	//set = !set;

	if(sld.state == isPreOn || sld.state == isSleepMode){
		print_dust_clck(0);
		/*hd44780_goto_xy(0, 15);
		dust_clock = (dust_clock==SYMB_DUST_CLOCK1) ? SYMB_DUST_CLOCK2 : SYMB_DUST_CLOCK1;
		hd44780_write_data(dust_clock);
		delay_ms(150);*/
	}
}

void printFenInfoLCD(u16 *airT){
	//static u16 oldAirT = -1;
	//static u8 dust_clock = 0;
	static u8 set = 0;

	hd44780_puts("Fen: ");
	print_temp_val(airT, &fen.temp, (encBtn == FEN_TEMP));
	/*hd44780_write_data(SYMB_TERM);
	if(cursor_cnt_state && encBtn == FEN_TEMP){
		hd44780_write_data((set) ? SYMB_SET_ARROW : 20);
		lcd_write_dec_auto(fen.temp);
	}else if(oldAirT < *airT){
		hd44780_write_data(SYMB_UP_ARROW);
		lcd_write_dec_auto(*airT);
	}else{
		hd44780_write_data(SYMB_DN_ARROW);
		lcd_write_dec_auto(*airT);
	}

	hd44780_write_data(SYMB_DEGREE);*/
	if(*airT < 10){
		hd44780_puts("  ");
	}else if(*airT < 100){
		hd44780_puts(" ");
	}

	hd44780_goto_xy(1, 11);

	if(cursor_cnt_state && encBtn == FEN_AIRFLOW && set){
		hd44780_write_data(SYMB_SET_ARROW);
	}else{
		hd44780_puts(" ");
	}
	lcd_write_dec_auto(fen.air_flow);
	hd44780_puts("%   ");

	//oldAirT = *airT;
	set = !set;

	if(fen.state == isPreOn || fen.state == isSleepMode){
		print_dust_clck(1);
		/*hd44780_goto_xy(1, 15);
		dust_clock = (dust_clock==SYMB_DUST_CLOCK1) ? SYMB_DUST_CLOCK2 : SYMB_DUST_CLOCK1;
		hd44780_write_data(dust_clock);
		delay_ms(150);*/
	}

}

void print_dust_clck(u8 row){
	static u8 dust_clock;
	dust_clock = (dust_clock==SYMB_DUST_CLOCK1) ? SYMB_DUST_CLOCK2 : SYMB_DUST_CLOCK1;

	hd44780_goto_xy(row, 15);
	hd44780_write_data(dust_clock);
	delay_ms(150);
}

void print_temp_val(u16 *temp, u16 *presetTemp, u8 cursor){
	static u8 set = 0;
	static u16 oldTemp = -1;

	hd44780_write_data(SYMB_TERM);
	if(cursor_cnt_state && cursor){
		hd44780_write_data((set) ? SYMB_SET_ARROW : 20);
		lcd_write_dec_auto(*presetTemp);
	}else{
		hd44780_write_data((oldTemp < *temp) ? SYMB_UP_ARROW : SYMB_DN_ARROW);
		lcd_write_dec_auto(*temp);
	}
	hd44780_write_data(SYMB_DEGREE);

	set = !set;
	oldTemp = *temp;
}

void init_user_chars(void){

	u8 user_char[8]; //���� ����� ���������� ���������������� ������

	user_char[0]=0b11000; // ������
	user_char[1]=0b11000; // ������ C
	user_char[2]=0b00000; //
	user_char[3]=0b00110; //
	user_char[4]=0b01001; //
	user_char[5]=0b01000; //
	user_char[6]=0b01001;
	user_char[7]=0b00110;

	hd44780_set_user_char(SYMB_DEGREE, user_char);

	user_char[0]=0b00000;
	user_char[1]=0b00100; // ������� �����
	user_char[2]=0b01110; //
	user_char[3]=0b10101; //
	user_char[4]=0b00100; //
	user_char[5]=0b00100; //
	user_char[6]=0b00100; //
	user_char[7]=0b00000;

	hd44780_set_user_char(SYMB_UP_ARROW, user_char);

	user_char[0]=0b11111;
	user_char[1]=0b10001; // �������� ����
	user_char[2]=0b01010; //
	user_char[3]=0b00100; //
	user_char[4]=0b01110; //
	user_char[5]=0b11111; //
	user_char[6]=0b11111; //
	user_char[7]=0b00000;

	hd44780_set_user_char(SYMB_DUST_CLOCK1, user_char);

	user_char[0]=0b11111;
	user_char[1]=0b11111; // �������� ����
	user_char[2]=0b01110; //
	user_char[3]=0b00100; //
	user_char[4]=0b01010; //
	user_char[5]=0b10001; //
	user_char[6]=0b11111; //
	user_char[7]=0b00000;

	hd44780_set_user_char(SYMB_DUST_CLOCK2, user_char);

	user_char[0]=0b00000;
	user_char[1]=0b00100; // ������� ������
	user_char[2]=0b00010; //
	user_char[3]=0b11111; //
	user_char[4]=0b00010; //
	user_char[5]=0b00100; //
	user_char[6]=0b00000; //
	user_char[7]=0b00000;

	hd44780_set_user_char(SYMB_SET_ARROW, user_char);

	user_char[0]=0b00000;
	user_char[1]=0b00100; // ���������
	user_char[2]=0b01110; //
	user_char[3]=0b00100; //
	user_char[4]=0b01110; //
	user_char[5]=0b00100; //
	user_char[6]=0b11111; //
	user_char[7]=0b01110;

	hd44780_set_user_char(SYMB_TERM, user_char);
}

void draw_logo(void){
	hd44780_goto_xy(0, 1);
	hd44780_puts("Solder station");
	hd44780_goto_xy(1, 1);
	hd44780_puts("Drovosekov.net");
	delay_ms(AFTER_LOGO_DELAY_ms);
}
