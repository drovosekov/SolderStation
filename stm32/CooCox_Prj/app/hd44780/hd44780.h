//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _HD44780_H
#define _HD44780_H

#include "main.h"

typedef enum {
	LCD_DISABLE = 0,
	LCD_ENABLE = !LCD_DISABLE
} lcd_state;

typedef enum {
	CURSOR_DISABLE = 0,
	CURSOR_ENABLE = !CURSOR_DISABLE
} cursor_state;


#define FLOAT_POINT	"."

// ������� ����������
#define HD44780_ROWS                    2       //4       // ���-�� �����
#define HD44780_COLS                    16      //20      // ���-�� ��������

#define HD44780_pcf8574_mode			1       //1       // ������ � �������� ����� i2c-������� �� pcf8574 (����� - ��������)

#define HD44780_4bitMode				1       // �� ������ - 4-��� ����� (����� 8-���) �������

#if (HD44780_pcf8574_mode)      // ���� �������� ����� i2c-������� �� pcf8574        
  
  // i2c-����� ���������� pcf8574 ������� ����������� �� ����� ��������:
  // 0x20-0x27 ��� pcf8574
  // 0x38-0x3F ��� pcf8574a.
  // ���������� ��������� - ������ ���. 1
  #define HD44780_pcf8574_addr          0x27    // 7-������ i2c-����� ���������� pcf8574(a)      
  #define HD44780_pcf8574_i2c_periph    I2C2    // I2C1 ��� I2C2
  
  #define HD44780_Data_Shift            4       // �� ������ - ������� ����� ������������ ���� ����� 
  
  #define HD44780_pcf8574_RS_mask       (1<<0)  // �� ������ - ������� ����� ������� RS �� ������ pcf8574
  #define HD44780_pcf8574_RW_mask       (1<<1)  // �� ������ - ������� ����� ������� RW �� ������ pcf8574
  #define HD44780_pcf8574_E_mask        (1<<2)  // �� ������ - ������� ����� ������� E �� ������ pcf8574
  #define HD44780_pcf8574_BL_mask       (1<<3)  // �� ������ - ������� ����� ������� ���������� ���������� �� ������ pcf8574

#else                           // ���� �������� �������� ����� GPIO ����������������

  //// ���� ����������������, ����������� ����������� hd44780
  // ������ RS
  #define HD44780_RS_Port         GPIOA
  #define HD44780_RS_Pin          GPIO_Pin_6
  // ������ RW
  #define HD44780_RW_Port         GPIOA
  #define HD44780_RW_Pin          GPIO_Pin_5
  // ������ E
  #define HD44780_E_Port          GPIOA
  #define HD44780_E_Pin           GPIO_Pin_4
  // ������ ���������� ���������� �������
  #define HD44780_BL_Port         GPIOA
  #define HD44780_BL_Pin          GPIO_Pin_7
  // ���� ������ ������������� ���������� hd44780
  #define HD44780_Data_Port       GPIOA

  #if HD44780_4bitMode
    #define HD44780_Data_Shift          0      // ������� ����� ������������ ���� ����� �� ����� ��
  #endif
#endif



#define HD44780_BackLightCtrl           1       // ���������� ���������� ������� (����� I2C-������� ��� ����� ����� GPIO ����������������)        
#define HD44780_WaitBisyFlag            1       // ����� ����� ��������� ����������� ������� (����� ����������� ��������). ���� HD44780_WaitBisyFlag=0, �� �� ���� ������� R/W ����� ������ ���������� ���������� 0
#define HD44780_ShortDelayUs            5       // �������� �������� (����� �������� E)
#define HD44780_BisyDelayUs             50      // �������� �������� ������������ ����������� ������� (���� HD44780_WaitBisyFlag=1)   

//#define CP1251                          1251	// ������������ ��������� ������ �������� ����� (��� ��������������� ���������)
//#define CP866                           866
//#define SOURCE_CODEPAGE                 CP1251



// ��������� ������������� �������
void hd44780_init(void);
// ��������� ���������� ���������� ������� (����� I2C-������� ��� ����� ����� GPIO)
void hd44780_backlight(uint8_t val);
// ��������� ������ ������� � ��������� ������������ ����������� �������
void hd44780_write_cmd(uint8_t Data);
// ��������� ������ ����� ������ � ��������� ������������ ����������� �������
void hd44780_write_data(uint8_t Data);
// ��������� ������ � ������� ������� ����
void hd44780_write_buff(char *pBuff, uint8_t Len);
// ��������� ������� �������
void hd44780_clear(void);
// ��������� ��������� ������� �������
void hd44780_goto_xy(uint8_t Row, uint8_t Col);
// ��������� �������� ANSI-������ � ������� ������� �������
void hd44780_puts(char *str);
// ��������� ���������������� ������ ������� � ������� ������� �������
void hd44780_printf(const char *format, ...);
// ��������� ������ � ������� ��������������� ��������
void hd44780_set_user_char(uint8_t char_num, uint8_t * char_data);
// ��������� ��������� ������ �������
void hd44780_set_state(lcd_state state, cursor_state cur_state);

void lcd_write_dec_auto(uint16_t dig);
void lcd_write_dec_xxxx(uint16_t dig);
void lcd_write_dec_xxx(uint16_t dig);
void lcd_write_dec_xx(uint8_t dig);
void lcd_write_dec_x(uint8_t dig);
void lcd_write_float(float dig);
#endif
