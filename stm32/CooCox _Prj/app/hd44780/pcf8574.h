//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _PCF8574_H
#define _PCF8574_H

#include "../types.h"


#define PCF8574_i2cRate         100000  // Частота работы i2c при работе с pcf8574
#define PCF8574_TO              1000    // Длительность ожидания таймаута операций на i2c при работе с pcf8574


// Функция считывает состояние линий pcf8574. Возвращает считанное значение или 0 - в случае неудачи 
uint8_t pcf8574_read(I2C_TypeDef* I2Cx, uint8_t slave_addr);
// Функция считывает состояние линий pcf8574 в буфер buff. Возвращает код ошибки (0 - в случае успеха) 
uint8_t pcf8574_read_ex(I2C_TypeDef* I2Cx, uint8_t slave_addr, uint8_t *buff);
// Функция записывает состояние линий pcf8574 из параметра value. Возвращает код ошибки (0 - в случае успеха) 
uint8_t pcf8574_write(I2C_TypeDef* I2Cx, uint8_t slave_addr, uint8_t value);
// Инициализация i2c для обмена с pcf8574
void pcf8574_bus_init(I2C_TypeDef* I2Cx);

#endif