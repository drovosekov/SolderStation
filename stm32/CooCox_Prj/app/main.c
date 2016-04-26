//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------

#include "main.h"

SLD_INFO sld;
SLD_INFO fen;

u8 count_do_beep = 0;		//���-�� ������
u16 power_off_count = 0;	//������� �������������� �������
u8 cursor_cnt_state = 0;	//������� �������������� ������� �������
EncBtnStates encBtn;

#define DEBUG	1

int main()
{
	//static EncoderModes modeSelected = selSolderTemperature;
	init_All();
	u16 airT = 0;
	u16 solderT = 0;

	while (1)
	{
		check_control_panel_buttons();

		if(count_do_beep){//��. � ����������� ���������� �� �������� Fen_Gerkon_IRQHandler � Sld_Gerkon_IRQHandler
			beep(count_do_beep);
			count_do_beep = 0;
		}

		//=========Solder=========
		hd44780_goto_xy(0, 0);

		solderT = get_solder_temp();
		switch(sld.state){
		case notReady:
			hd44780_puts("Sld is out stand");
			break;
		case isOn:
		case isPreOn:
		case isSleepMode:
			printSolderInfoLCD(&solderT);
			break;
		case isOff:
			hd44780_puts("Sld: =off=       ");
			break;
		}
		//========================

		//========Fen Solder======
		hd44780_goto_xy(1, 0);

		airT = get_airfen_temp();
		switch(fen.state){
		case notReady:
			hd44780_puts("Fen is out stand");
			break;
		case isOn:
		case isPreOn:
		case isSleepMode:
			printFenInfoLCD(&airT);
			break;
		case isOff:
			hd44780_puts("Fen: =off=       ");
			break;
		}
		//========================

		//hd44780_goto_xy(0, 14);
		//lcd_write_dec_auto(cursor_cnt_state);
	}
}

void beep(u8 count){
	while(count--){
		buzzer(ENABLE);
		delay_ms(BEEP_DELAY_ms);
		buzzer(DISABLE);
		if(count) {delay_ms(BEEP_DELAY_ms);}
	}
}

void buzzer(u8 state){
	TIM_Cmd(TIM4, state);
}


//���������� ���������� �� ������� 2 - ����������� 1 ��� � �������
void TIM2_IRQHandler(void)
{
	TIM_ClearFlag(TIM2, TIM_SR_UIF);//���������� ���� ����������

	PIN_REVERSE(USER_LED_green);	//������ ������ ��� � ������� ����������� �� ����� �����������

	//==���������� �������������� �������==
	if((fen.state == isOff || fen.state == notReady) &&
	   (sld.state == isOff || sld.state == notReady)){
		power_off_count++;
		if(power_off_count > AUTO_POWER_OFF){
			PIN_OFF(RELAY_POWER);
		}
	}else{
		power_off_count = 0;
	}
	//=====================================

	//====������ ���������� �� ������ ������������� ��� ������� ������===
	if(fen.state == isPreOn || fen.state == isSleepMode){
		fen.auto_off++;
		if(fen.auto_off > AUTO_FEN_OFF){
			fen.state = isOff;
		}
	}else{
		fen.auto_off = 0;
	}

	if(sld.state == isPreOn || sld.state == isSleepMode){
		sld.auto_off++;
		if(sld.auto_off > AUTO_SLD_OFF){
			sld.state = isOff;
		}
	}else{
		sld.auto_off = 0;
	}
	//===================================================================


	if(cursor_cnt_state > 1){
		cursor_cnt_state--;
	}else if(cursor_cnt_state == 1){
		cursor_cnt_state = 0;
		encBtn = FEN_TEMP;
	}
}


void init_All(void){

	SystemInit();

	delay_init();		//��������� ��������

	mcu_gpio_init();	//������������� ������

	hd44780_init();		//������������� LCD �������

	init_user_chars();	//�������� ���������������� ��������

	init_adc();			//������������� ���

	external_init();	//������������� ������������ ����������

	hd44780_backlight(SET);//�������� ��������� ������

#if !DEBUG
	draw_logo();		//������ �������
	beep(3);			//��� ���� ����
#endif

	hd44780_clear();

	PIN_ON(RELAY_POWER);	//��������� ���� ������� �������

	sld.state = (PIN_STATE(GERKON_SOLDER)) ? isOff : notReady;
	fen.state = (PIN_STATE(GERKON_AIR))    ? isOff : notReady;

	init_tim();			//������������� ��������
}


