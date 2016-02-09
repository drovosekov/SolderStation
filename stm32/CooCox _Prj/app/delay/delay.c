//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "main.h"

//Timer Functions
static u32 delay = 0;

//==============================================================================
// ������ ����������� ������ 1���
//==============================================================================
void SysTick_Handler(void)
{
	static u16 led = 0;

	if(delay){delay--;}

	if(++led >= 50000){
		led=0;
		PIN_REVERSE(USER_LED_green);
	}
}

//==============================================================================
// ��������� ����������� �������� 1 ���
//==============================================================================
void delay_us(u32 ms)
{
	delay = ms;
	while(delay){}
}
//==============================================================================

//==============================================================================
// ��������� ����������� �������� 1 ��
//==============================================================================
void delay_ms(u32 ms)
{
	delay = ms * 100;
	while(delay){}
}
//==============================================================================
