//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "main.h"
#include "i2cm.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if (HD44780_pcf8574_mode)
  #include "pcf8574.h"
  uint8_t pcf8574_Value = 0;
#endif

  //массив для преобразования чисел при прямом выводе их на дисплей
 const u8 lcd_decode[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  
// Макросы для управления выходами
#if (HD44780_pcf8574_mode)
  // Макросы для управления интерфейсом при работе через I2C-адаптер на pcf8574 
  #define HD44780_RS_HIGH()             {pcf8574_Value |= HD44780_pcf8574_RS_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_RS_LOW()              {pcf8574_Value &= ~HD44780_pcf8574_RS_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_RW_HIGH()             {pcf8574_Value |= HD44780_pcf8574_RW_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_RW_LOW()              {pcf8574_Value &= ~HD44780_pcf8574_RW_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_E_HIGH()              {pcf8574_Value |= HD44780_pcf8574_E_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_E_LOW()               {pcf8574_Value &= ~HD44780_pcf8574_E_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_BL_HIGH()             {pcf8574_Value |= HD44780_pcf8574_BL_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_BL_LOW()              {pcf8574_Value &= ~HD44780_pcf8574_BL_mask; pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_SetDATA_4bit(val)     {pcf8574_Value &= ~(0xF << HD44780_Data_Shift); pcf8574_Value |= ((val & 0xF) << HD44780_Data_Shift); pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
  #define HD44780_GetDATA_4bit()        ((pcf8574_read(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr) >> HD44780_Data_Shift) & 0xF)
  #define HD44780_SetDATA_PinMode_In(mask)      {pcf8574_Value |= (mask); pcf8574_write(HD44780_pcf8574_i2c_periph, HD44780_pcf8574_addr, pcf8574_Value);}
#else
  // Макросы для управления интерфейсом при работе напрямую через GPIO микроконтроллера 
  #define HD44780_RS_HIGH()             GPIO_SetBits(HD44780_RS_Port, HD44780_RS_Pin)
  #define HD44780_RS_LOW()              GPIO_ResetBits(HD44780_RS_Port, HD44780_RS_Pin)
  #define HD44780_RW_HIGH()             GPIO_SetBits(HD44780_RW_Port, HD44780_RW_Pin)
  #define HD44780_RW_LOW()              GPIO_ResetBits(HD44780_RW_Port, HD44780_RW_Pin)
  #define HD44780_E_HIGH()              GPIO_SetBits(HD44780_E_Port, HD44780_E_Pin)
  #define HD44780_E_LOW()               GPIO_ResetBits(HD44780_E_Port, HD44780_E_Pin)
  #define HD44780_BL_HIGH()             GPIO_SetBits(HD44780_BL_Port, HD44780_BL_Pin)
  #define HD44780_BL_LOW()              GPIO_ResetBits(HD44780_BL_Port, HD44780_BL_Pin)
  #define HD44780_SetDATA_4bit(val)     {GPIO_ResetBits(HD44780_Data_Port, 0xF << HD44780_Data_Shift); GPIO_SetBits(HD44780_Data_Port, val << HD44780_Data_Shift);}
  #define HD44780_SetDATA_8bit(val)     {GPIO_ResetBits(HD44780_Data_Port, 0xFF); GPIO_SetBits(HD44780_Data_Port, val);}
  #define HD44780_GetDATA_4bit()        ((GPIO_ReadInputData(HD44780_Data_Port) >> HD44780_Data_Shift) & 0xF)
  #define HD44780_GetDATA_8bit()        (GPIO_ReadInputData(HD44780_Data_Port) && 0xFF)
  #define HD44780_SetDATA_PinMode_In(mask)      hd44780_SetGPIOmode_In(HD44780_Data_Port, mask)
  #define HD44780_SetDATA_PinMode_Out(mask)     hd44780_SetGPIOmode_Out(HD44780_Data_Port, mask)
#endif

uint8_t LastRow = 0;                                               // Последняя установленная строка
char hd44780_StrBuff[HD44780_COLS * HD44780_ROWS + 8];          // Буфер строки для вывода


#if (!HD44780_pcf8574_mode)
//==============================================================================
// Процедура настройки пинов микроконтроллера (по масте) как входов 
//==============================================================================
void hd44780_SetGPIOmode_In(GPIO_TypeDef* GPIOx, uint16_t Mask)
{
  GPIO_InitTypeDef InitStruct;
  InitStruct.GPIO_Pin = Mask;
  InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  InitStruct.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOx, &InitStruct);
}
//==============================================================================


//==============================================================================
// Процедура настройки пинов микроконтроллера (по масте) как выходов 
//==============================================================================
void hd44780_SetGPIOmode_Out(GPIO_TypeDef* GPIOx, uint16_t Mask)
{
  GPIO_InitTypeDef InitStruct;
  InitStruct.GPIO_Pin = Mask;
  InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOx, &InitStruct);
}
//==============================================================================
#endif

//==============================================================================
// Функция чтения байта из hd44780
// - IsCmd - чтение статусного байта (иначе байта данных)
//==============================================================================
uint8_t hd44780_read(int8_t IsCmd)
{
  unsigned char Data = 0;
  
  if (IsCmd)
  {
    HD44780_RS_LOW();
  }
  else
  {
    HD44780_RS_HIGH();
  }
  
  HD44780_RW_HIGH();
  
#if HD44780_4bitMode
  // Считываем старшую тетраду
  HD44780_E_HIGH();
  delay_us(HD44780_ShortDelayUs);
  Data = HD44780_GetDATA_4bit() << 4;
  HD44780_E_LOW();

  delay_us(HD44780_ShortDelayUs);

  // Считываем младшую тетраду
  HD44780_E_HIGH();
  delay_us(HD44780_ShortDelayUs);
  Data |= HD44780_GetDATA_4bit();
  HD44780_E_LOW();
#else
  HD44780_E_HIGH();
  delay_us(HD44780_ShortDelayMks);
  // Считываем байт с шины данных
  Data = HD44780_GetDATA_8bit();
  HD44780_E_LOW();
#endif
  
  HD44780_RW_LOW();

  return Data;
}
//==============================================================================


//==============================================================================
// Процедура записи байта из hd44780
// - IsCmd - запись командного байта (иначе байта данных)
//==============================================================================
void hd44780_write(uint8_t Data, int8_t IsCmd)
{
  if (IsCmd)
  {
    HD44780_RS_LOW();
  }
  else
  {
    HD44780_RS_HIGH();
  }
  
#if HD44780_4bitMode
  #if (!HD44780_pcf8574_mode)
    // Настраиваем биты данных как выходы
    HD44780_SetDATA_PinMode_Out(0xF << HD44780_Data_Shift);
  #endif
  // Выдаём старшую тетраду
  HD44780_SetDATA_4bit(Data >> 4);
  HD44780_E_HIGH();
  delay_ms(HD44780_ShortDelayUs);
  HD44780_E_LOW();

  delay_us(HD44780_ShortDelayUs);

  // Выдаём младшую тетраду
  HD44780_SetDATA_4bit(Data & 0xF);
  HD44780_E_HIGH();
  delay_ms(HD44780_ShortDelayUs);
  HD44780_E_LOW();
  
  // Настраиваем биты данных как входы
  HD44780_SetDATA_PinMode_In(0xF << HD44780_Data_Shift);
#else
  #if (!HD44780_pcf8574_mode)
    // Настраиваем биты данных как выходы
    HD44780_SetDATA_PinMode_Out(0xFF);
  #endif
  
  // Выдаём байт
  HD44780_SetDATA_8bit(Data);
  HD44780_E_HIGH();
  delay_us(HD44780_ShortDelayUs);
  HD44780_E_LOW();
  
  // Настраиваем биты данных как входы
  HD44780_SetDATA_PinMode_In(0xFF);
#endif
}
//==============================================================================


//==============================================================================
// Функция ожидания освобождения контроллера дисплея (если HD44780_WaitBisyFlag=1)
//==============================================================================
#if (HD44780_WaitBisyFlag)
uint8_t hd44780_waitbisy(uint8_t tick)
{
  while ((hd44780_read(1) & 0x80) && (tick)) 
  {
    tick--;
  }
  
  return (tick) ? 0 : 1;
}
#endif
//==============================================================================


//==============================================================================
// Процедура записи команды с ожиданием освобождения контроллера дисплея
//==============================================================================
void hd44780_write_cmd(uint8_t Data)
{
#if (HD44780_WaitBisyFlag)
  hd44780_waitbisy(100);
#endif
  
  hd44780_write(Data, 1);

#if (!HD44780_WaitBisyFlag)
  delay_us(HD44780_BisyDelayUs);
#endif
}
//==============================================================================


//==============================================================================
// Процедура записи байта данных с ожиданием освобождения контроллера дисплея
//==============================================================================
void hd44780_write_data(uint8_t Data)
{
#if (HD44780_WaitBisyFlag)
  hd44780_waitbisy(100);
#endif

  hd44780_write(Data, 0);

#if (!HD44780_WaitBisyFlag)
  delay_us(HD44780_BisyDelayUs);
#endif
}
//==============================================================================

#if (!HD44780_pcf8574_mode)
//==============================================================================
// Процедура старта тактирования GPIO
//==============================================================================
void PortClockStart(GPIO_TypeDef *GPIOx)
{
  if (GPIOx == GPIOA)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  else if (GPIOx == GPIOB)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  else if (GPIOx == GPIOC)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  else if (GPIOx == GPIOD)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  else if (GPIOx == GPIOE)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  else if (GPIOx == GPIOF)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
  else if (GPIOx == GPIOG)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
}
//==============================================================================
#endif

//==============================================================================
// Процедура управления подсветкой дисплея (через I2C-адаптер или через ножки GPIO)
//==============================================================================
void hd44780_backlight(uint8_t val)
{
  if (val){
    HD44780_BL_HIGH();
  }else{
    HD44780_BL_LOW();
  }
}
//==============================================================================


//==============================================================================
// Процедура инициализации ног GPIO/I2C-адаптера для работы с hd44780
//==============================================================================
void hd44780_bus_init(void)
{
#if (HD44780_pcf8574_mode)
  pcf8574_bus_init(HD44780_pcf8574_i2c_periph);
#else
  // Стартуем тактирование периферии портов
  PortClockStart(HD44780_RS_Port);
  PortClockStart(HD44780_RW_Port);
  PortClockStart(HD44780_E_Port);
  PortClockStart(HD44780_BL_Port);
  PortClockStart(HD44780_Data_Port);
  
  // Все ноги, управляющие шиной настраиваем как выходы
  // Сигнал E
  hd44780_SetGPIOmode_Out(HD44780_E_Port, HD44780_E_Pin);
  HD44780_E_LOW();
  // Сигнал RS
  hd44780_SetGPIOmode_Out(HD44780_RS_Port, HD44780_RS_Pin);
  HD44780_RS_LOW();

#if (HD44780_BackLightCtrl)
  // Сигнал управления подсветкой
  hd44780_SetGPIOmode_Out(HD44780_BL_Port, HD44780_BL_Pin);
  HD44780_BL_LOW();
#endif
  
#if (HD44780_WaitBisyFlag)
  // Сигнал RW
  hd44780_SetGPIOmode_Out(HD44780_RW_Port, HD44780_RW_Pin);
#else
  /*
  hd44780_SetGPIOmode_Out(HD44780_RW_Port, HD44780_RW_Pin);
  GPIO_ResetBits(HD44780_RW_Port, HD44780_RW_Pin);
  */
#endif
  
#endif
}
//==============================================================================

//==============================================================================
// Процедура инициализации дисплея (отправка последовательности команд в дисплей)
//==============================================================================
void hd44780_start(void)
{
	delay_ms(15);
	hd44780_write_cmd(0x30); //reset lcd
	delay_ms(5);
	hd44780_write_cmd(0x30); //reset lcd
	delay_ms(1);
	hd44780_write_cmd(0x30); //reset lcd
	delay_ms(1);

#if HD44780_4bitMode
  hd44780_write_cmd(0x02);       // Размер символа, ширина шина данных
  //uint8_t Reg = 0x20;
//#else
//  uint8_t Reg = 0x30;
#endif
/*
#if (HD44780_ROWS > 1)
  Reg |= 0x08;
#endif
  */
  //hd44780_write_cmd(Reg);       // Размер символа, ширина шина данных
  //delay_ms(1);
  hd44780_write_cmd(0x0C);      // Включаем дисплей
  delay_ms(1);
  hd44780_write_cmd(0x06);      // Автоинкремент адреса
  delay_ms(1);
  hd44780_clear();              // Очистка экрана
}
//==============================================================================


//==============================================================================
// Процедура инициализации дисплея
//==============================================================================
void hd44780_init(void)
{
  hd44780_bus_init();
  delay_ms(100);
  hd44780_start();
}
//==============================================================================


//==============================================================================
// Процедура установки курсора дисплея
//==============================================================================
void hd44780_goto_xy(uint8_t Row, uint8_t Col)
{
  // Вычисляем начальный адрес нужной строки
  uint8_t Adr = 0;            
  if (Row & 1)                  // Нечётные строки с т.з. hd44780 - вторая строка
    Adr = 0x40;                 // делаем смещение до второй строки hd44780
  if (Row > 1)                  // Для 4-строчного дисплея при выборе строки > 2
    Adr += HD44780_COLS;        // делаем сдвиг адреса на длину строки

  // Добавляем сдвиг в строке
  Adr += Col;
  
  // Пишем полученный адрес DRAM в hd44780
  hd44780_write_cmd(Adr | 0x80);        // команда установки адреса в DRAM
  
  LastRow = Row;
}
//==============================================================================


//==============================================================================
// Процедура очистки дисплея
//==============================================================================
void hd44780_clear(void)
{
  hd44780_write_cmd(0x01);      // Очистка экрана

#if (!HD44780_WaitBisyFlag)
  delay_ms(2);
#endif
}
//==============================================================================


//==============================================================================
// Процедура записи в дисплей массива байт
//==============================================================================
void hd44780_write_buff(char *pBuff, uint8_t Len)
{
  while (Len--)
  {
    hd44780_write_data(*(pBuff++));
  }
}
//==============================================================================


//==============================================================================
// Процедура вывода числовых значений
//==============================================================================
void lcd_write_dec_auto(uint16_t dig){
	if(dig<10){
		lcd_write_dec_x((u8)dig);
	}else if(dig<100){
		lcd_write_dec_xx((u8)dig);
	}else if(dig<1000){
		lcd_write_dec_xxx(dig);
	}else {
		lcd_write_dec_xxxx(dig);
	}
}
void lcd_write_dec_xxxx(uint16_t dig){
	u16 d=dig % 1000;
	hd44780_write_data(lcd_decode[(dig / 1000) & 0x0F]);
	hd44780_write_data(lcd_decode[(d / 100) & 0x0F]);
	hd44780_write_data(lcd_decode[(d % 100) / 10 & 0x0F]);
	hd44780_write_data(lcd_decode[(d % 100) % 10 & 0x0F]);
}

void lcd_write_dec_xxx(uint16_t dig){
	hd44780_write_data(lcd_decode[(dig / 100) & 0x0F]);
	hd44780_write_data(lcd_decode[((dig % 100) / 10) & 0x0F]);
	hd44780_write_data(lcd_decode[((dig % 100) % 10) & 0x0F]);
}

void lcd_write_dec_xx(uint8_t dig){
	hd44780_write_data(lcd_decode[((dig % 100) / 10) & 0x0F]);
	hd44780_write_data(lcd_decode[((dig % 100) % 10) & 0x0F]);
}

void lcd_write_dec_x(uint8_t dig) {
	hd44780_write_data(lcd_decode[dig]);
}

void lcd_write_float(float dig){//dig - дробное число с точностью до десятых долей
	u8 dig_int = dig; //здесь будет целая часть
	lcd_write_dec_xx(dig_int);

	hd44780_puts(FLOAT_POINT);

	u8 dig_float = (dig - dig_int) * 10; //здесь будет дробная часть
	lcd_write_dec_x(dig_float);
}
//==============================================================================

//==============================================================================
// Процедура отправки ANSI-строки в текущую позицию дисплея
//==============================================================================
void hd44780_puts(char *str)
{
  char i;
  
  while (*str != '\0')
  {
    switch (*str)
    {
    case '\n':  // Переход на новую строку
      LastRow++;
      hd44780_goto_xy(LastRow, 0);
      break;
    case '\t':
      for (i = 0; i < 4; i++)
        hd44780_write_data(0x20);
      break;
    default:
        hd44780_write_data(*str);
      break;
    }
    str++;
  }
}
//==============================================================================


//==============================================================================
// Процедура форматированного вывода начиная с текущей позиции курсора
//==============================================================================
#ifdef SOURCE_CODEPAGE
	void hd44780_printf(const char *args, ...)
	{
	  va_list ap;
	  va_start(ap, args);
	  vsnprintf(hd44780_StrBuff, sizeof(hd44780_StrBuff), args, ap);
	  va_end(ap);

	  // Перекодирование русских символов в кодировку EPSON для дисплеев hd44780
	#if (SOURCE_CODEPAGE == CP1251)
	  cp1251_to_epson_convert(hd44780_StrBuff);
	#else
	  cp866_to_epson_convert(hd44780_StrBuff);
	#endif

	  hd44780_puts(hd44780_StrBuff);
	}
#else
	void hd44780_printf(const char *args, ...)
	{
	  va_list ap;
	  va_start(ap, args);
	  vsnprintf(hd44780_StrBuff, sizeof(hd44780_StrBuff), args, ap);
	  va_end(ap);

	  hd44780_puts(hd44780_StrBuff);
	}
#endif


//==============================================================================

//==============================================================================
// Процедура записи в дисплей пользовательских символов
//==============================================================================
void hd44780_set_user_char(uint8_t char_num, uint8_t * char_data) {
	u8 i;
	hd44780_write_cmd(((1<<6) | (char_num * 8) ));
	for (i = 0; i < 8 ; i++) {
		hd44780_write_data(char_data[i]);
	}
	hd44780_write_cmd((1<<7));
}
//==============================================================================
