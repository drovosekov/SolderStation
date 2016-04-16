//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
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

// Размеры индикатора
#define HD44780_ROWS                    2       //4       // Кол-во строк
#define HD44780_COLS                    16      //20      // Кол-во столбцов

#define HD44780_pcf8574_mode			1       //1       // Работа с дисплеем через i2c-адаптер на pcf8574 (иначе - напрямую)

#define HD44780_4bitMode				1       // НЕ МЕНЯТЬ - 4-бит режим (иначе 8-бит) дисплея

#if (HD44780_pcf8574_mode)      // Если работаем через i2c-адаптер на pcf8574        
  
  // i2c-адрес микросхемы pcf8574 задаётся перемычками на плате адаптера:
  // 0x20-0x27 для pcf8574
  // 0x38-0x3F для pcf8574a.
  // Отсутствие перемычки - обычно лог. 1
  #define HD44780_pcf8574_addr          0x27    // 7-битный i2c-адрес микросхемы pcf8574(a)      
  #define HD44780_pcf8574_i2c_periph    I2C2    // I2C1 или I2C2
  
  #define HD44780_Data_Shift            4       // НЕ МЕНЯТЬ - Битовый сдвиг параллельной шины влево 
  
  #define HD44780_pcf8574_RS_mask       (1<<0)  // НЕ МЕНЯТЬ - Битовая маска сигнала RS на линиях pcf8574
  #define HD44780_pcf8574_RW_mask       (1<<1)  // НЕ МЕНЯТЬ - Битовая маска сигнала RW на линиях pcf8574
  #define HD44780_pcf8574_E_mask        (1<<2)  // НЕ МЕНЯТЬ - Битовая маска сигнала E на линиях pcf8574
  #define HD44780_pcf8574_BL_mask       (1<<3)  // НЕ МЕНЯТЬ - Битовая маска сигнала управления подсветкой на линиях pcf8574

#else                           // Если работаем напрямую через GPIO микроконтроллера

  //// Ноги микроконтроллера, управляющие интерфейсом hd44780
  // Сигнал RS
  #define HD44780_RS_Port         GPIOA
  #define HD44780_RS_Pin          GPIO_Pin_6
  // Сигнал RW
  #define HD44780_RW_Port         GPIOA
  #define HD44780_RW_Pin          GPIO_Pin_5
  // Сигнал E
  #define HD44780_E_Port          GPIOA
  #define HD44780_E_Pin           GPIO_Pin_4
  // Сигнал управления подсветкой дисплея
  #define HD44780_BL_Port         GPIOA
  #define HD44780_BL_Pin          GPIO_Pin_7
  // Ноги данных параллельного интерфейса hd44780
  #define HD44780_Data_Port       GPIOA

  #if HD44780_4bitMode
    #define HD44780_Data_Shift          0      // Битовый сдвиг параллельной шины влево по порту МК
  #endif
#endif



#define HD44780_BackLightCtrl           1       // Управление подсветкой дисплея (через I2C-адаптер или через ножку GPIO микроконтроллера)        
#define HD44780_WaitBisyFlag            1       // Опрос флага занятости контроллера дисплея (иначе программная задержка). Если HD44780_WaitBisyFlag=0, то на ногу дисплея R/W нужно подать постоянный логический 0
#define HD44780_ShortDelayUs            5       // Короткая задержка (между фронтами E)
#define HD44780_BisyDelayUs             50      // Задержка ожидания освобождения контроллера дисплея (если HD44780_WaitBisyFlag=1)   

//#define CP1251                          1251	// Используемая кодировка файлов исходных кодов (для перекодирования кириллицы)
//#define CP866                           866
//#define SOURCE_CODEPAGE                 CP1251



// Процедура инициализации дисплея
void hd44780_init(void);
// Процедура управления подсветкой дисплея (через I2C-адаптер или через ножки GPIO)
void hd44780_backlight(uint8_t val);
// Процедура записи команды с ожиданием освобождения контроллера дисплея
void hd44780_write_cmd(uint8_t Data);
// Процедура записи байта данных с ожиданием освобождения контроллера дисплея
void hd44780_write_data(uint8_t Data);
// Процедура записи в дисплей массива байт
void hd44780_write_buff(char *pBuff, uint8_t Len);
// Процедура очистки дисплея
void hd44780_clear(void);
// Процедура установки курсора дисплея
void hd44780_goto_xy(uint8_t Row, uint8_t Col);
// Процедура отправки ANSI-строки в текущую позицию дисплея
void hd44780_puts(char *str);
// Процедура форматированного вывода начиная с текущей позиции курсора
void hd44780_printf(const char *format, ...);
// Процедура записи в дисплей ползовательских символов
void hd44780_set_user_char(uint8_t char_num, uint8_t * char_data);
// Процедура установки режима дисплея
void hd44780_set_state(lcd_state state, cursor_state cur_state);

void lcd_write_dec_auto(uint16_t dig);
void lcd_write_dec_xxxx(uint16_t dig);
void lcd_write_dec_xxx(uint16_t dig);
void lcd_write_dec_xx(uint8_t dig);
void lcd_write_dec_x(uint8_t dig);
void lcd_write_float(float dig);
#endif
