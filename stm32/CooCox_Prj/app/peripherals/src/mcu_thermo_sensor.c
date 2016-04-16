#include "main.h"


#ifdef SENS_3WIRE
s8 get_temperature_3wire(){
	u8 buf[2];

    if(OW_Send(OW_SEND_RESET, (u8 *)"\xcc\x44", 2, 0, 0, OW_NO_READ) == OW_NO_DEVICE)
    	{return 0;}

#if DS18B20_RESOLUTION == DS18B20_9BIT
    delay_for(100);
#endif
#if DS18B20_RESOLUTION == DS18B20_10BIT
    delay_for(200);
#endif
#if DS18B20_RESOLUTION == DS18B20_11BIT
    delay_for(400);
#endif
#if DS18B20_RESOLUTION == DS18B20_12BIT
    delay_for(750);
#endif

    OW_Send(OW_SEND_RESET, (u8 *)"\xcc\xbe\xff\xff", 4, (u8 *)buf, 2, 2);

	buf[0] >>= 4; // убираем дробную часть
	buf[1] <<= 4; // убираем лишние знаки

	return(buf[1] | buf[0]); // объединяем 2 байта -> возврат
}
#endif

#ifdef SENS_2WIRE
//не отлажено на железе!!!
void get_temperature_2wire(int *hb, int *lb){
	//команда всем сенсорам - измерять
    if(OW_Send(OW_SEND_RESET, (u8 *)"\xcc\x44", 2, 0, 0, OW_NO_READ)==OW_NO_DEVICE)
    	{return;}

	//PIN_ON(LED_GREEN);
    // назначаем функцию двухтактного выхода - подаем "питание" на шину
    OW_out_set_as_Power_pin();

    // выдерживаем время измерения (например 750 мс для 12-битного измерения)
#if DS18B20_RESOLUTION == DS18B20_9BIT
    delay_for(100);
#endif
#if DS18B20_RESOLUTION == DS18B20_10BIT
    delay_for(200);
#endif
#if DS18B20_RESOLUTION == DS18B20_11BIT
    delay_for(400);
#endif
#if DS18B20_RESOLUTION == DS18B20_12BIT
    delay_for(750);
#endif

    // восстанавливаем функцию передатчика UART
    OW_out_set_as_TX_pin();

	u8 buf[2];
    OW_Send(OW_SEND_RESET, (u8 *)"\xcc\xbe\xff\xff", 4, (u8 *)buf, 2, 2);

#if 0
	buf[0] >>= 4; // убираем дробную часть
	buf[1] <<= 4; // убираем лишние знаки

	return(buf[1] | buf[0]); // объединяем 2 байта -> возврат
#endif

	get_temperature_value(buf, *hb, *lb);
}

void get_temperature_value(u8 *buf, int hb, int lb){
    //определяем знак
    if ((buf[1] & 0x80) != 0) {    //если старший разряд не 0 то..
    	hb=-hb; //ставим знак "-"
        unsigned int tmp;        //выполняем необходимые преобразования с числом
        tmp = ((unsigned int)buf[1]<<8) | buf[0]; //соединяем 0 и 1 байт ОЗУ сенсора
        tmp = ~tmp + 1; //выполняем опреацию логического отрицания
        buf[0] = tmp; //возврящаем результат преобразования
        buf[1] = tmp>>8;
    }
    hb = ((buf[1] & 7)<<4)|(buf[0]>>4);//целая часть
    lb = (buf[0] & 15);//определяем остаток
    lb = (lb<<2) + (hb<<3);// умножаем на 10
    lb = (lb>>4);//делим на 16 или умножаем на 0.0625
}
#endif
