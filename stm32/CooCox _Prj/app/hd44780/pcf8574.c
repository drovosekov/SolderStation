//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <stm32f10x_i2c.h>
#include "pcf8574.h"
#include "i2cm.h"


//==============================================================================
// Функция считывает состояние линий pcf8574. Возвращает считанное значение или 0 - в случае неудачи 
//==============================================================================
uint8_t pcf8574_read(I2C_TypeDef* I2Cx, uint8_t slave_addr)
{
  uint8_t val = 0;
  
  // Выдаём START на шину
  if (i2cm_Start(I2Cx, slave_addr, 1, PCF8574_TO))
  {
    i2cm_Stop(I2Cx, PCF8574_TO);
    pcf8574_bus_init(I2Cx);
    return 0;
  }
  // Читаем 1 байт
  if (i2cm_ReadBuffAndStop(I2Cx, &val, 1, PCF8574_TO))
    return 0;
  
  return val;
}
//==============================================================================


//==============================================================================
// Функция считывает состояние линий pcf8574 в буфер buff. Возвращает код ошибки (0 - в случае успеха) 
//==============================================================================
uint8_t pcf8574_read_ex(I2C_TypeDef* I2Cx, uint8_t slave_addr, uint8_t *buff)
{
  int8_t err;
  
  // Выдаём START на шину
  err = i2cm_Start(I2Cx, slave_addr, 1, PCF8574_TO);
  if (err)
  {
    i2cm_Stop(I2Cx, PCF8574_TO);
    pcf8574_bus_init(I2Cx);
    return err;
  }
  // Читаем 1 байт
  err = i2cm_ReadBuffAndStop(I2Cx, buff, 1, PCF8574_TO);
  if (err)
    return err;
  
  return I2C_ERR_Ok;
}
//==============================================================================


//==============================================================================
// Функция записывает состояние линий pcf8574 из параметра value. Возвращает код ошибки (0 - в случае успеха) 
//==============================================================================
uint8_t pcf8574_write(I2C_TypeDef* I2Cx, uint8_t slave_addr, uint8_t value)
{
  int8_t err;
  
  // Выдаём START на шину
  err = i2cm_Start(I2Cx, slave_addr, 0, PCF8574_TO);
  if (err)
  {
    i2cm_Stop(I2Cx, PCF8574_TO);
    pcf8574_bus_init(I2Cx);
    return err;
  }
  
  err = i2cm_WriteBuff(I2Cx, &value, 1, PCF8574_TO);
  i2cm_Stop(I2Cx, PCF8574_TO);
  if (err)
    return err;
  
  return I2C_ERR_Ok;
}
//==============================================================================


//==============================================================================
// Инициализация i2c для обмена с pcf8574
//==============================================================================
void pcf8574_bus_init(I2C_TypeDef* I2Cx)
{
  i2cm_init(I2Cx, PCF8574_i2cRate);
}
//==============================================================================
