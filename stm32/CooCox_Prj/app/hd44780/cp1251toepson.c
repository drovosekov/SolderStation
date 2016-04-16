//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "cp1251toepson.h"

#ifdef SOURCE_CODEPAGE
//==============================================================================
// ��������� ��������� ������� ��������� � ��������� 1251 �� ������� � �������� 
// EPSON ��� hd44780 
//==============================================================================
void cp1251_to_epson_convert(char *str)
{
	#if (SOURCE_CODEPAGE == CP1251)
  while (*str != '\0')
  {
    switch (*str)
    {
    case '�': *str = 0x41; break;      // �
    case '�': *str = 0xA0; break;      // �
    case '�': *str = 0x42; break;      // �
    case '�': *str = 0xA1; break;      // �
    case '�': *str = 0xE0; break;      // �
    case '�': *str = 0x45; break;      // �
    case '�': *str = 0xA2; break;      // �
    case '�': *str = 0xA3; break;      // �
    case '�': *str = 0x33; break;      // �
    case '�': *str = 0xA5; break;      // �
    case '�': *str = 0xA6; break;      // �
    case '�': *str = 0x4B; break;      // �
    case '�': *str = 0xA7; break;      // �
    case '�': *str = 0x4D; break;      // �
    case '�': *str = 0x48; break;      // �
    case '�': *str = 0x4F; break;      // �
    case '�': *str = 0xA8; break;      // �
    case '�': *str = 0x50; break;      // �
    case '�': *str = 0x43; break;      // �
    case '�': *str = 0x54; break;      // �
    case '�': *str = 0xA9; break;      // �
    case '�': *str = 0xAA; break;      // �
    case '�': *str = 0x58; break;      // �
    case '�': *str = 0xE1; break;      // �
    case '�': *str = 0xAB; break;      // �
    case '�': *str = 0xAC; break;      // �
    case '�': *str = 0xE2; break;      // �
    case '�': *str = 0xAD; break;      // �
    case '�': *str = 0xAE; break;      // �
    case '�': *str = 0x62; break;      // �
    case '�': *str = 0xAF; break;      // �
    case '�': *str = 0xB0; break;      // �
    case '�': *str = 0xB1; break;      // �
    case '�': *str = 0x61; break;      // �
    case '�': *str = 0xB2; break;      // �
    case '�': *str = 0xB3; break;      // �
    case '�': *str = 0xB4; break;      // �
    case '�': *str = 0xE3; break;      // �
    case '�': *str = 0x65; break;      // �
    case '�': *str = 0xB5; break;      // �
    case '�': *str = 0xB6; break;      // �
    case '�': *str = 0xB7; break;      // �
    case '�': *str = 0xB8; break;      // �
    case '�': *str = 0xB9; break;      // �
    case '�': *str = 0xBA; break;      // �
    case '�': *str = 0xBB; break;      // �
    case '�': *str = 0xBC; break;      // �
    case '�': *str = 0xBD; break;      // �
    case '�': *str = 0x6F; break;      // �
    case '�': *str = 0xBE; break;      // �
    case '�': *str = 0x70; break;      // �
    case '�': *str = 0x63; break;      // �
    case '�': *str = 0xBF; break;      // �
    case '�': *str = 0x79; break;      // �
    case '�': *str = 0xE4; break;      // �
    case '�': *str = 0x78; break;      // �
    case '�': *str = 0xE5; break;      // �
    case '�': *str = 0xC0; break;      // �
    case '�': *str = 0xC1; break;      // �
    case '�': *str = 0xE6; break;      // �
    case '�': *str = 0xC2; break;      // �
    case '�': *str = 0xC3; break;      // �
    case '�': *str = 0xC4; break;      // �
    case '�': *str = 0xC5; break;      // �
    case '�': *str = 0xC6; break;      // �
    case '�': *str = 0xC7; break;      // �
    }
    str++;
  }
#endif
}
//==============================================================================
#endif
