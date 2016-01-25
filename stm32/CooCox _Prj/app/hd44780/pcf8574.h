//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _PCF8574_H
#define _PCF8574_H

#include "../types.h"


#define PCF8574_i2cRate         100000  // ������� ������ i2c ��� ������ � pcf8574
#define PCF8574_TO              1000    // ������������ �������� �������� �������� �� i2c ��� ������ � pcf8574


// ������� ��������� ��������� ����� pcf8574. ���������� ��������� �������� ��� 0 - � ������ ������� 
uint8_t pcf8574_read(I2C_TypeDef* I2Cx, uint8_t slave_addr);
// ������� ��������� ��������� ����� pcf8574 � ����� buff. ���������� ��� ������ (0 - � ������ ������) 
uint8_t pcf8574_read_ex(I2C_TypeDef* I2Cx, uint8_t slave_addr, uint8_t *buff);
// ������� ���������� ��������� ����� pcf8574 �� ��������� value. ���������� ��� ������ (0 - � ������ ������) 
uint8_t pcf8574_write(I2C_TypeDef* I2Cx, uint8_t slave_addr, uint8_t value);
// ������������� i2c ��� ������ � pcf8574
void pcf8574_bus_init(I2C_TypeDef* I2Cx);

#endif